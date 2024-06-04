#include "Components/Movement/ChasingComponent.h"
#include "Actors/Units/MoveableUnit.h"

UChasingComponent::UChasingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

/**
 * Starts chasing the specified target. Triggers the FOnReachingTargetEvent when within the maximum distance.
 * For targets that can't move, it only moves to the location once without adjusting the path over time.
 * @param NewTarget The unit to be chased.
 * @param MaxDistance The maximum distance to the target at which FOnReachingTargetEvent will be triggered.
 */
void UChasingComponent::BeginChase(ABaseUnit* NewTarget, uint32 MaxDistance)
{
	if(AMoveableUnit* MoveableTarget = Cast<AMoveableUnit>(NewTarget))
	{
		BeginChase(MoveableTarget, MaxDistance);
		return;
	}
	StaticCast<AMoveableUnit*>(GetOwner())->MoveTo(NewTarget->GetActorLocation2d());
}

/**
 * Starts chasing the specified target. Triggers the FOnReachingTargetEvent when within the maximum distance.
 * For moving units, it adjusts the path every 0.1 seconds unless the target is removed from the world.
 * TODO: Needs to be tested for a large number of units chasing a target at a large distance with many obstacles,as the current approach is computationally expensive and requires optimization. * @param NewTarget The unit to be chased.
 * @param MaxDistance The maximum distance to the target at which FOnReachingTargetEvent will be triggered.
 */
void UChasingComponent::BeginChase(AMoveableUnit* NewTarget, uint32 MaxDistance)
{
	MaxDistanceSqr = MaxDistance * MaxDistance;
	Target = NewTarget;
	FTimerDelegate ChasingDelegate;
	ChasingDelegate.BindUFunction(this, "MoveTowardsTarget");
	GetWorld()->GetTimerManager().SetTimer(ChasingTimer, ChasingDelegate, 0.1f, true);
	NewTarget->OnEndPlay.AddUniqueDynamic(this, &UChasingComponent::OnTargetDestroyed);
}

/**
 * Ends the chasing process, removes all active timers, and unregisters all listeners associated with the current chase.
 */
void UChasingComponent::EndChase()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(ChasingTimer))
	{
		GetWorld()->GetTimerManager().ClearTimer(ChasingTimer);
	}
	if (IsValid(Target))
	{
		Target->OnEndPlay.RemoveDynamic(this, &UChasingComponent::OnTargetDestroyed);
	}
	Target = nullptr;
	OnReachingTargetEvent.Clear();
}

/**
 * Stops the chasing if the owner's state changes to anything other than chasing. Calls EndChase().
 */
void UChasingComponent::OwnerPrepareToStateChangeListener(TEnumAsByte<EUnitState> NewState, TEnumAsByte<EUnitState> OldState)
{
	if (Target == nullptr)
	{
		return;
	}
	if (NewState != Chasing && NewState != Chasing_Stop)
	{
		EndChase();
	}
}

void UChasingComponent::BeginPlay()
{
	Super::BeginPlay();
	check(GetOwner()->IsA(AMoveableUnit::StaticClass()))
	StaticCast<AMoveableUnit*>(GetOwner())->GetPrepareUnitStateEventHandler().AddUFunction(this, "OwnerPrepareToStateChangeListener");
}

/**
 * Moves the owner towards its target.
 * If it has already reached the desired distance, it will stay in a chasing_stop state and be ready to chase again as soon as the target moves.
 * Calls HasReachedTarget() to check the distance.
 */
void UChasingComponent::MoveTowardsTarget()
{
	if (bIsReached)
	{
		StaticCast<AMoveableUnit*>(GetOwner())->Stop(Chasing_Stop);
		FRotator ActorRotation = (Target->GetActorLocation() - GetOwner()->GetActorLocation()).Rotation();
		ActorRotation.Add(0, -90, 0);
		GetOwner()->SetActorRotation(ActorRotation);
	}
	else
	{
		StaticCast<AMoveableUnit*>(GetOwner())->MoveTo(Target->GetActorLocation2d() + Target->GetVelocity2d() * 0.1,Chasing);
	}
	HasReachedTarget();
}

void UChasingComponent::OnTargetDestroyed(AActor* Actor, EEndPlayReason::Type EndPlayReason)
{
	EndChase();
}

/**
 * Checks if the owner has reached its target. If it has, changes the component status(bIsReached) to indicate the target is reached
 * and broadcasts the OnReachingTargetEvent. Otherwise, keeps bIsReached set to false.
 */
void UChasingComponent::HasReachedTarget()
{
	if (FVector2d::DistSquared(FVector2d(GetOwner()->GetActorLocation()), Target->GetActorLocation2d()) <= MaxDistanceSqr)
	{
		OnReachingTargetEvent.Broadcast();
		bIsReached = true;
		return;
	}
	bIsReached = false;
}
