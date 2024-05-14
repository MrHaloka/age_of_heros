#include "Actors/Units/BuildingConstruction.h"

#include "Components/BuilderComponent.h"
#include "DataStructures/BuildingInfo.h"
#include "Factories/UnitInfoFactory.h"
#include "GamePlay/GameStatics.h"
#include "GamePlay/PlayerSpectatorPawn.h"
#include "GamePlay/RTSGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/ObjectsManager.h"
#include "Components/PlayerResourceManagerComponent.h"

ABuildingConstruction::ABuildingConstruction()
{
	PrimaryActorTick.bCanEverTick = true;

	HologramMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hologram Mesh"));
	HologramMesh->SetMobility(EComponentMobility::Static);

	UnderConstructionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("UnderConstruction Mesh"));
	UnderConstructionMesh->SetMobility(EComponentMobility::Static);
	UnderConstructionMesh->SetVisibility(false);

	SetRootComponent(HologramMesh);
}

FVector2d ABuildingConstruction::GetSize()
{
	return GameStatics::GetBuildingReferenceByID(GameStatics::GetClassTypeId(this->GetClass()->GetFName()))->GetSize();
}

void ABuildingConstruction::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (BStarted)
	{
		UpdateBuildingProgress(DeltaSeconds);
	}
}

void ABuildingConstruction::BeginPlay()
{
	Super::BeginPlay();
	TimeToComplete = BuildingInfo->TimeToComplete;
	APlayerSpectatorPawn* PlayerSpectatorPawn = CastChecked<APlayerSpectatorPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	PlayerSpectatorPawn->GetComponentByClass<UPlayerResourceManagerComponent>()->Pay(GetPrice());
	CalculateBuildersLocations();
	UnderConstructionMesh->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
}

float ABuildingConstruction::GetMaxHP()
{
	return BuildingInfo->MaxHP;
}

FResources ABuildingConstruction::GetPrice()
{
	return BuildingInfo->GetPrice();
}

void ABuildingConstruction::AddVillagerToBuild(UBuilderComponent* BuilderComponent)
{
	ActiveBuilders.Add(&*BuilderComponent);
	VillagerCounter++;
	if (VillagerCounter == 1)
	{
		PrimaryActorTick.SetTickFunctionEnable(true);
	}
}

void ABuildingConstruction::RemoveVillagerFromBuilding(UBuilderComponent* BuilderComponent, const FVector2d& BuilderLocation)
{
	ActiveBuilders.Remove(&*BuilderComponent);
	VillagerCounter--;
	BuildersGridLocations.Add(BuilderLocation);
	if (VillagerCounter == 0)
	{
		PrimaryActorTick.SetTickFunctionEnable(false);
	}
}

void ABuildingConstruction::UpdateBuildingProgress(float DeltaSeconds)
{
	TimeToComplete -= (DeltaSeconds*(VillagerCounter+2)) / 3;
	if (TimeToComplete <= 0)
	{
		Finish();
	}
}

void ABuildingConstruction::Finish()
{
	BroadcastBuildingCompletion();
	Destroy();
	SpawnTheBuilding();
}

void ABuildingConstruction::Refund()
{
	APlayerSpectatorPawn* PlayerSpectatorPawn = CastChecked<APlayerSpectatorPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	PlayerSpectatorPawn->GetComponentByClass<UPlayerResourceManagerComponent>()->Refund(GetPrice() * (TimeToComplete / BuildingInfo->TimeToComplete));
}

void ABuildingConstruction::Initialize(const FBuildingInfo* NewBuildingInfo, ETeams NewPlayerTeam)
{
	BuildingInfo = NewBuildingInfo;
	SetTeam(NewPlayerTeam);
}

const FBuildingInfo* ABuildingConstruction::GetBuildingInfo() const
{
	return GameStatics::GetBuildingReferenceByID(GameStatics::GetClassTypeId(this->GetClass()->GetFName()));
}

bool ABuildingConstruction::IsStarted()
{
	return BStarted;
}

void ABuildingConstruction::Starting()
{
	BStarted = true;
	HologramMesh->SetVisibility(false, false);
	UnderConstructionMesh->SetVisibility(true);
	GameStatics::GetObjectManager()->RemoveHologram(GetID());
	if (ARTSHUD* RTSHUD = GetWorld()->GetFirstPlayerController()->GetPawn<APlayerSpectatorPawn>()->GetRTSHUD())
	{
		RTSHUD->UnitAdded(GetActorLocation2d(), FColor::Blue);
	}
}

TOptional<FVector2d> ABuildingConstruction::GetClosestEmptyBuilderLocation(const FVector2d& BuilderLocation)
{
	uint32 MinDistance = TNumericLimits<int32>::Max();
	TOptional<FVector2d> Location = NullOpt;
	for (FVector2d BuildersGridLocation : BuildersGridLocations)
	{
		if (GameStatics::GetObjectManager()->IsGridBlock(BuildersGridLocation))
		{
			continue;
		}
		uint32 CurrentDistance = FVector2d::DistSquared(BuildersGridLocation, BuilderLocation);
		if (CurrentDistance < MinDistance)
		{
			MinDistance = CurrentDistance;
			Location = BuildersGridLocation;
		}
	}
	if (Location.IsSet())
	{
		BuildersGridLocations.Remove(Location.GetValue());
	}
	return Location;
}

void ABuildingConstruction::ReturnSlotLocation(const FVector2d& SlotLocation)
{
	BuildersGridLocations.Add(SlotLocation);
}

void ABuildingConstruction::SpawnTheBuilding()
{
	GameStatics::GetObjectManager()->SpawnUnit(
		GetActorLocation(),
		FUnitInfoFactory(BuildingInfo->UnitClass,BuildingInfo->GetSize()),
		GetTeam()
		);
}

void ABuildingConstruction::CalculateBuildersLocations()
{
	FVector2d SideDirections[4] = {
		FVector2d(1, 0),
		FVector2d(0, 1),
		FVector2d(-1, 0),
		FVector2d(0, -1)
	};
	const int32 CellSize = GameStatics::GetObjectManager()->GetCellSize();
	FVector2d StartingCorner = GetActorLocation2d();
	FVector2d Offset = (CellSize / 2) * SideDirections[3];
	for (int Side = 0; Side < 4; Side++)
	{
		FVector2d EndingCorner = StartingCorner + SideDirections[Side] * BuildingInfo->GetSize();
		FVector2d TempLocation = StartingCorner;
		while (TempLocation != EndingCorner)
		{
			BuildersGridLocations.Add(TempLocation + Offset);
			TempLocation = TempLocation + SideDirections[Side] * CellSize;
		}
		BuildersGridLocations.Add(EndingCorner + Offset);
		StartingCorner = EndingCorner;
		Offset = (CellSize / 2) * SideDirections[Side];
	}
}

void ABuildingConstruction::BroadcastBuildingCompletion()
{
	for (UBuilderComponent* Builder : ActiveBuilders)
	{
		Builder->OnBuildingCompleted();
	}
}
