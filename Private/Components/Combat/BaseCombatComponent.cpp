
#include "Components/Combat/BaseCombatComponent.h"

#include "Actors/Units/MoveableUnit.h"
#include "Actors/Units/Buildings/BaseBuilding.h"
#include "Components/AI/SightComponent.h"
#include "Engine/DamageEvents.h"

UBaseCombatComponent::UBaseCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bIsOnReload = false;
	Target = nullptr;
}

/**
 * Checks if the unit has a sight component and tries to listen for the SenseNewUnit event.
 * Adds a listener for the OwnerStateChange event.
 */
void UBaseCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	if (USightComponent* SightComponent = StaticCast<ABaseUnit*>(GetOwner())->GetComponentByClass<USightComponent>())
	{
		SightComponent->OnSenseUnitEvent.AddUFunction(this, "SensedNewUnitListener");
	}
	StaticCast<ABaseUnit*>(GetOwner())->GetPrepareUnitStateEventHandler().AddUFunction(this, "OwnerPrepareToStateChangeListener");
}

/**
 * Upon sensing a new unit, checks if it is an enemy. If it is, calls StartAttack().
 * new target will be ignored as long as the old target is active.
 */
void UBaseCombatComponent::SensedNewUnitListener(ABaseUnit* Unit)
{
	if (GetOwnerState() != Idle && GetOwnerState() != Patrolling)
	{
		return;
	}
	if (StaticCast<ABaseUnit*>(GetOwner())->GetTeam() != Unit->GetTeam() && !Target)
	{
		StartAttack(Unit);
	}
}

/**
 * Starts attacking the enemy unit and resets the old target.
 * The behavior depends on the owner type (building/movable).
 */
void UBaseCombatComponent::StartAttack(ABaseUnit* EnemyUnit)
{
	if (Target)
	{
		ResetTarget();
	}
	
	if (GetOwner()->IsA(ABaseBuilding::StaticClass()))
	{
		BuildingStartAttack(EnemyUnit);
	}
	else if (GetOwner()->IsA(AMoveableUnit::StaticClass()))
	{
		MoveableStartAttack(EnemyUnit);
	}
}

/**
 * Starts attacking an enemy unit for owner type buildings.
 * Only initiates the attack if it is not reloading and the target is within range.
 */
void UBaseCombatComponent::BuildingStartAttack(ABaseUnit* EnemyUnit)
{
	check(GetOwner()->IsA(ABaseBuilding::StaticClass()))
	if (bIsOnReload)
	{
		return;
	}
	if (FVector2d::DistSquared(FVector2d(GetOwner()->GetActorLocation()), EnemyUnit->GetActorLocation2d()) <= MaxRange * MaxRange)
	{
		SetTarget(EnemyUnit);
		Attack();
	}
}

/**
 * Starts attacking an enemy unit for movable owner types.
 * Checks the distance and activates chasing the target.
 * Only initiates the attack if the target is within range.
 */
void UBaseCombatComponent::MoveableStartAttack(ABaseUnit* EnemyUnit)
{
	check(GetOwner()->IsA(AMoveableUnit::StaticClass()))
	SetTarget(EnemyUnit);
	if (FVector2d::DistSquared(FVector2d(GetOwner()->GetActorLocation()), EnemyUnit->GetActorLocation2d()) <= MaxRange * MaxRange)
	{
		Attack();
	}
	AMoveableUnit* MoveableOwner = StaticCast<AMoveableUnit*>(GetOwner());
	MoveableOwner->Chase(EnemyUnit, MaxRange);
	MoveableOwner->GetOnReachingTargetEventHandler().AddUFunction(this, "ReachedTargetListener");
}

/**
 * Sets the target. This should be used instead of directly changing targets.
 */
void UBaseCombatComponent::SetTarget(ABaseUnit* NewTarget)
{
	NewTarget->OnEndPlay.AddUniqueDynamic(this, &UBaseCombatComponent::ResetTarget);
	Target = NewTarget;
}

/**
 * Performs the attack. The default implementation deals direct damage.
 * Override this method for ranged or other types of attacks based on your needs.
 */
void UBaseCombatComponent::PerformAttack()
{
	const FDamageEvent DamageEvent;
	Target->TakeDamage(Damage, DamageEvent, GetWorld()->GetFirstPlayerController(), GetOwner());
}

/**
 * Calls the perform attack method and changes the class status to reloading.
 * To change any attack behavior, it is recommended to modify the perform attack method 
 * instead of this one, as it handles the default behavior of any attack, such as the reload timer and more.
 */
void UBaseCombatComponent::Attack()
{
	PerformAttack();
	bIsOnReload = true;
	FTimerHandle TimerHandle;
	FTimerDelegate ReloadDelegate;
	ReloadDelegate.BindUFunction(this, "ResetReload");
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, ReloadDelegate, ReloadTime + AttackLength, false);
}

EUnitState UBaseCombatComponent::GetOwnerState()
{
	return StaticCast<ABaseUnit*>(GetOwner())->GetUnitState();
}

void UBaseCombatComponent::SetOwnerState(EUnitState UnitState)
{
	StaticCast<ABaseUnit*>(GetOwner())->SetUnitState(UnitState);
}

void UBaseCombatComponent::ResetTarget(AActor* DestroyedActor, EEndPlayReason::Type EndPlayReason)
{
	ResetTarget();
}

void UBaseCombatComponent::ResetTarget()
{
	UE_LOG(LogTemp, Warning, TEXT("target reset called"))
	if (IsValid(Target))
	{
		Target->OnEndPlay.RemoveDynamic(this, &UBaseCombatComponent::ResetTarget);
	}
	Target = nullptr;
	SetOwnerState(Idle);
}

float UBaseCombatComponent::GetReloadTime()
{
	return ReloadTime;
}

float UBaseCombatComponent::GetCooldown()
{
	return Cooldown;
}

void UBaseCombatComponent::OwnerPrepareToStateChangeListener(TEnumAsByte<EUnitState> NewState, TEnumAsByte<EUnitState> OldState)
{
	if (Target == nullptr)
	{
		return;
	}
	if (NewState != Attacking && NewState != Patrolling && NewState != Chasing && NewState != Chasing_Stop)
	{
		ResetTarget();
	}
}

void UBaseCombatComponent::ReachedTargetListener()
{
	if (!bIsOnReload)
	{
		Attack();	
	}
}

void UBaseCombatComponent::ResetReload()
{
	bIsOnReload = false;
	if (GetOwner()->IsA(ABaseBuilding::StaticClass()))
	{
		FindAndAttackTarget();
	}
}

/**
 * Only applicable for owners of type ABaseBuilding.
 * Tries to attack the old target if it is still available and in range. If it is not, then it tries to find a new one.
 * This method is called after the building has reloaded its weapon and is ready to attack again.
 */
void UBaseCombatComponent::FindAndAttackTarget()
{
	check(GetOwner()->IsA(ABaseBuilding::StaticClass()))
	if (Target && FVector2d::DistSquared(FVector2d(GetOwner()->GetActorLocation()), Target->GetActorLocation2d()) <= MaxRange * MaxRange)
	{
		Attack();
		return;
	}
	ResetTarget();
	if (USightComponent* OwnerSightComponent = GetOwner()->GetComponentByClass<USightComponent>())
	{
		if (ABaseUnit* NewTarget = OwnerSightComponent->GetFirstEnemyInPerception())
		{
			Target = NewTarget;
			Target->OnEndPlay.AddUniqueDynamic(this, &UBaseCombatComponent::ResetTarget);
			Attack();
		}
	}
}