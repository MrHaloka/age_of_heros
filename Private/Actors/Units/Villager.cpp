#include "Actors/Units/Villager.h"

#include "Components/DropoffComponent.h"
#include "Enums/UnitState.h"
#include "GamePlay/RTSGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/DropoffsManager.h"
#include "Managers/ObjectsManager.h"
#include "Actors/Resources/BaseResources.h"
#include "UI/RTSHUD.h"
#include "Actors/Units/BuildingConstruction.h"
#include "Actors/Units/Buildings/BaseBuilding.h"
#include "Components/BuilderComponent.h"
#include "Components/ResourceHarvestingComponent.h"

AVillager::AVillager() : AMoveableUnit()
{
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal"));
}

void AVillager::BeginPlay()
{
	Super::BeginPlay();
	SetRootComponent(SkeletalMeshComponent);
	
	UResourceHarvestingComponent* ResourceHarvestingComponent = NewObject<UResourceHarvestingComponent>(this, UResourceHarvestingComponent::StaticClass(), TEXT("Harvester"), RF_NoFlags, UResourceHarvestingComponent::StaticClass()->GetDefaultObject(), true);
	ResourceHarvestingComponent->RegisterComponent();
	AddInstanceComponent(ResourceHarvestingComponent);

	UBuilderComponent* BuilderComponent = NewObject<UBuilderComponent>(this, UBuilderComponent::StaticClass(),TEXT("Builder"), RF_NoFlags, UBuilderComponent::StaticClass()->GetDefaultObject(), true);
	BuilderComponent->RegisterComponent();
	AddInstanceComponent(BuilderComponent);
}

void AVillager::GatherResource(ABaseResources* Resource)
{
	GetComponentByClass<UResourceHarvestingComponent>()->Initialize(Resource);
}

void AVillager::EndActivityAndMove()
{
	if (UnitState == Harvesting || UnitState == Moving_To_Resources)
	{
		GetComponentByClass<UResourceHarvestingComponent>()->StopHarvesting();
	}
	if (UnitState == Building || UnitState == Moving_To_Building)
	{
		GetComponentByClass<UBuilderComponent>()->StopBuilding();
	}
}

void AVillager::OnPrepareUnitStateChange(TEnumAsByte<EUnitState> NewState)
{
	Super::OnPrepareUnitStateChange(NewState);
	if (NewState == Moving)
	{
		EndActivityAndMove();
	}
}

void AVillager::GoToConstructionSite(ABuildingConstruction* ConstructionBase)
{
	GetComponentByClass<UBuilderComponent>()->Initialize(ConstructionBase);
}

void AVillager::AddToInventory(const FSingleResource& NewResource)
{
	Inventory += NewResource;
}

const FInventory& AVillager::GetInventory() const
{
	return Inventory;
}

void AVillager::SetDropOffPoint(UDropoffComponent* NewDropOff)
{
	DropOffPoint = NewDropOff;
}

void AVillager::ShowUnitPanel(ARTSHUD* RTSHUD)
{
	Super::ShowUnitPanel(RTSHUD);
	RTSHUD->ShowVillagerPanel(this);
}

void AVillager::ReachedDropoff()
{
	DropOffPoint->DroppedResources(Inventory.GetData());
	Inventory.Reset();
	GetComponentByClass<UResourceHarvestingComponent>()->GoBackToResource();
}

void AVillager::ReachedTargetActor()
{
	if (UnitState == Moving_To_Resources)
	{
		GetComponentByClass<UResourceHarvestingComponent>()->SetComponentTickEnabled(true);
		SetUnitState(Harvesting);
	}
	if (UnitState == Moving_To_Building)
	{
		GetComponentByClass<UBuilderComponent>()->StartBuilding();
		SetUnitState(Building);
	}
	if (UnitState == Moving_To_Dropoff)
	{
		ReachedDropoff();
	}
}

void AVillager::FinalPathfindingGoalReached()
{
	Super::FinalPathfindingGoalReached();
	if (UnitState != Moving)
	{
		ReachedTargetActor();
	}
}

void AVillager::GoBackToDropoff()
{
	if (ARTSGameMode* GameMode = Cast<ARTSGameMode>(UGameplayStatics::GetGameMode(GEngine->GetCurrentPlayWorld())))
	{
		ABaseUnit* Unit = GameMode->GetDropoffManager()->GetClosestDropOff(GetActorLocation2d(), Inventory.ResourceType);
		if (Unit != nullptr)
		{
			TArray<UDropoffComponent*> DropoffComponents;
			Unit->GetComponents<UDropoffComponent>(DropoffComponents);
			for (UDropoffComponent* Dropoff : DropoffComponents)
			{
				if (Dropoff->GetResourceType() == Inventory.ResourceType)
				{
					DropOffPoint = Dropoff;
					break;
				}
			}
			check(DropOffPoint != nullptr)
			check(Unit->IsA(ABaseBuilding::StaticClass()))
			ABaseBuilding* Building = StaticCast<ABaseBuilding*>(Unit);
			MoveTo(Building->GetClosestEmptySlot(GetActorLocation2d()), Moving_To_Dropoff);
		}
	}
}