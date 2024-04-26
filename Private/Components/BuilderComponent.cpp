#include "Components/BuilderComponent.h"
#include "Actors/Units/BuildingConstruction.h"
#include "Actors/Units/MoveableUnit.h"
#include "GamePlay/GameStatics.h"

UBuilderComponent::UBuilderComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UBuilderComponent::BeginPlay()
{
	Super::BeginPlay();
	check(GetOwner()->IsA(AMoveableUnit::StaticClass()))
}

void UBuilderComponent::Initialize(ABuildingConstruction* ConstructionBase)
{
	TOptional<FVector2d> BuilderLocation = ConstructionBase->GetClosestEmptyBuilderLocation(StaticCast<AMoveableUnit*>(GetOwner())->GetActorLocation2d());
	if (BuilderLocation.IsSet())
	{
		SlotLocation = BuilderLocation.GetValue();
		BuildingTarget = &*ConstructionBase;
		StaticCast<AMoveableUnit*>(GetOwner())->MoveTo(BuilderLocation.GetValue(), Moving_To_Building);
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
	StaticCast<AMoveableUnit*>(GetOwner())->SetUnitState(Idle);
	UE_LOG(LogTemp, Warning, TEXT("villager is idle now"));
}

