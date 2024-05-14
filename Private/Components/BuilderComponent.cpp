#include "Components/BuilderComponent.h"
#include "Actors/Units/BuildingConstruction.h"
#include "Actors/Units/MoveableUnit.h"
#include "Actors/Units/Villager.h"
#include "GamePlay/GameStatics.h"

UBuilderComponent::UBuilderComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UBuilderComponent::OwnerPrepareToStateChangeListener(TEnumAsByte<EUnitState> NewState, TEnumAsByte<EUnitState> OldState)
{
	if (OldState == Building)
	{
		StopBuilding();
	}
	if (OldState == Moving_To_Building && NewState != Building)
	{
		StopGoingTo();
	}
}

void UBuilderComponent::OnPathfindingGoalReachedListener()
{
	if (StaticCast<AMoveableUnit*>(GetOwner())->GetUnitState() == Moving_To_Building)
	{
		StartBuilding();
		StaticCast<AMoveableUnit*>(GetOwner())->SetUnitState(Building);
	}
}

void UBuilderComponent::BeginPlay()
{
	Super::BeginPlay();
	check(GetOwner()->IsA(AVillager::StaticClass()))
	StaticCast<AMoveableUnit*>(GetOwner())->GetPrepareUnitStateEventHandler().AddUFunction(this, "OwnerPrepareToStateChangeListener");
	StaticCast<AMoveableUnit*>(GetOwner())->GetFinalPathfindingGoalReachEventHandler().AddUFunction(this, "OnPathfindingGoalReachedListener");

}

void UBuilderComponent::Initialize(ABuildingConstruction* ConstructionBase)
{
	TOptional<FVector2d> BuilderLocation = ConstructionBase->GetClosestEmptyBuilderLocation(StaticCast<AVillager*>(GetOwner())->GetActorLocation2d());
	if (BuilderLocation.IsSet())
	{
		StaticCast<AVillager*>(GetOwner())->MoveTo(BuilderLocation.GetValue(), Moving_To_Building);
		SlotLocation = BuilderLocation.GetValue();
		BuildingTarget = &*ConstructionBase;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("There's no place left for vill"))
		// TODO: Implement the part of the pathfinding algorithm that finds the closest available path.
	}
}

void UBuilderComponent::StopBuilding()
{
	if (BuildingTarget!= nullptr && IsValid(BuildingTarget))
	{
		BuildingTarget->RemoveVillagerFromBuilding(this, SlotLocation);
	}
}

void UBuilderComponent::StopGoingTo()
{
	if (BuildingTarget!= nullptr && IsValid(BuildingTarget))
	{
		BuildingTarget->ReturnSlotLocation(SlotLocation);
	}
}

void UBuilderComponent::StartBuilding()
{
	if (!BuildingTarget->IsStarted())
	{
		const FBuildingConstructComponent ConstructComponent(GameStatics::GetObjectManager());
		const FBuildingInfo *BuildingInfo = GameStatics::GetBuildingReferenceByID(BuildingTarget->GetClassID());
		if (!ConstructComponent.CanStartBuilding(BuildingInfo,BuildingTarget->GetActorLocation2d(), BuildingTarget->GetID()))
		{
			return;
		}
		BuildingTarget->Starting();
	}
	BuildingTarget->AddVillagerToBuild(this);
}

void UBuilderComponent::OnBuildingCompleted()
{
	StaticCast<AVillager*>(GetOwner())->SetUnitState(Idle);
	UE_LOG(LogTemp, Warning, TEXT("villager is idle now"));
}

