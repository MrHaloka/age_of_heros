
#include "Actors/Units/Buildings/BaseBuilding.h"

#include "DataStructures/BuildingInfo.h"
#include "GamePlay/GameStatics.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/ObjectsManager.h"

ABaseBuilding::ABaseBuilding()
{
	PrimaryActorTick.bCanEverTick = false;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Building Mesh"));
	StaticMesh->SetMobility(EComponentMobility::Stationary);
	SetRootComponent(StaticMesh);
}

UStaticMeshComponent& ABaseBuilding::GetMeshComponent()
{
	return *StaticMesh;
}

FResources ABaseBuilding::GetPrice()
{
	return GameStatics::GetBuildingReferenceByID(GameStatics::GetClassTypeId(this->GetClass()->GetFName()))->GetPrice();
}

FVector2d ABaseBuilding::GetClosestEmptySlot(const FVector2d& TargetLocation)
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
	uint32 MinDistance = TNumericLimits< int32 >::Max();
	FVector2d Result = StartingCorner+Offset;
	for (int Side = 0; Side < 4; Side++)
	{
		uint32 TempDistance;
		FVector2d EndingCorner = StartingCorner + SideDirections[Side] * GetBuildingInfo()->GetSize();
		FVector2d TempLocation = StartingCorner;
		while (TempLocation != EndingCorner)
		{
			TempDistance = FVector2d::DistSquared(TempLocation + Offset, TargetLocation);
			if (MinDistance > TempDistance)
			{
				MinDistance = TempDistance;
				Result = TempLocation + Offset;
			}
			TempLocation = TempLocation + SideDirections[Side] * CellSize;
		}
		TempDistance = FVector2d::DistSquared(TempLocation + Offset, TargetLocation);
		if (MinDistance > TempDistance)
		{
			MinDistance = TempDistance;
			Result = TempLocation + Offset;
		}
		StartingCorner = EndingCorner;
		Offset = (CellSize / 2) * SideDirections[Side];
	}
	return Result;
}

FVector2d ABaseBuilding::GetSize()
{
	return GameStatics::GetBuildingReferenceByID(GameStatics::GetClassTypeId(this->GetClass()->GetFName()))->GetSize();
}

float ABaseBuilding::GetMaxHP()
{
	return GameStatics::GetBuildingReferenceByID(GameStatics::GetClassTypeId(this->GetClass()->GetFName()))->MaxHP;
}

void ABaseBuilding::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseBuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseBuilding::ShowUnitPanel(ARTSHUD* TargetHUD)
{
	Super::ShowUnitPanel(TargetHUD);
}

const FBuildingInfo* ABaseBuilding::GetBuildingInfo() const
{
	return  GameStatics::GetBuildingReferenceByID(GameStatics::GetClassTypeId(this->GetClass()->GetFName()));
}