#include "Components/ResourceHarvestingComponent.h"
#include "Actors/Units/MoveableUnit.h"
#include "DataStructures/ResourcesInfo.h"
#include "Enums/UnitState.h"
#include "GamePlay/RTSGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/HarvestingManager.h"
#include "Actors/Resources/BaseResources.h"

UResourceHarvestingComponent::UResourceHarvestingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UResourceHarvestingComponent::Initialize(ABaseResources* Resource)
{
	if (Resource == nullptr)
	{
		StaticCast<AVillager*>(GetOwner())->SetUnitState(Idle);
		return;
	}
	SlotLocation = HarvestingManager->GetResourceLocation(StaticCast<AVillager*>(GetOwner())->GetActorLocation2d(), Resource);
	ResourceId = Resource->GetID();
	ResourceInfo = Resource->GetResourceInfo();
	StaticCast<AVillager*>(GetOwner())->MoveTo(SlotLocation, Moving_To_Resources);
	DrawDebugBox(GetWorld(), FVector(SlotLocation, 0), FVector(25), FColor::Green, false, 1.5, 0, 10);
	TargetResource = Resource;
}

void UResourceHarvestingComponent::StopHarvesting()
{
	SetComponentTickEnabled(false);
	HarvestingManager->VillagerStoppedHarvesting(ResourceId, SlotLocation);
}

void UResourceHarvestingComponent::GoBackToResource()
{
	Initialize(IsValid(TargetResource)? TargetResource : GetNewResourceTarget());
}

void UResourceHarvestingComponent::OwnerPrepareToStateChangeListener(TEnumAsByte<EUnitState> NewState, TEnumAsByte<EUnitState> OldState)
{
	if (OldState == Harvesting)
	{
		StopHarvesting();
	}
	else if (OldState == Moving_To_Resources && NewState != Harvesting)
	{
		StopHarvesting();
	}
}

void UResourceHarvestingComponent::BeginPlay()
{
	Super::BeginPlay();
	if (ARTSGameMode* GameMode = Cast<ARTSGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		HarvestingManager = GameMode->GetHarvestingManager();
	}
	SetComponentTickEnabled(false);
	check(GetOwner()->IsA(AVillager::StaticClass()))
	StaticCast<ABaseUnit*>(GetOwner())->GetPrepareUnitStateEventHandler().AddUFunction(this, "OwnerPrepareToStateChangeListener");
}

void UResourceHarvestingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (StaticCast<AVillager*>(GetOwner())->GetInventory().IsFull())
	{
		StopHarvesting();
		StaticCast<AVillager*>(GetOwner())->GoBackToDropoff();
		return;
	}
	if (!IsValid(TargetResource))
	{
		SetComponentTickEnabled(false);
		Initialize(GetNewResourceTarget());
		return;
	}
	StaticCast<AVillager*>(GetOwner())->AddToInventory(FSingleResource(ResourceInfo->PerSecond * DeltaTime, ResourceInfo->Type));
	TargetResource->RemoveFromResource(ResourceInfo->PerSecond * DeltaTime);
}

ABaseResources* UResourceHarvestingComponent::GetNewResourceTarget()
{
	return HarvestingManager->GetNewResource(StaticCast<AVillager*>(GetOwner())->GetActorLocation2d(), ResourceInfo->Type);
}


