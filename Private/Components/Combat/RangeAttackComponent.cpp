#include "Components/Combat/RangeAttackComponent.h"
#include "Actors/Projectils/BaseProjectile.h"
#include "Actors/Units/MoveableUnit.h"
#include "Components/AI/SightComponent.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"


void URangeAttackComponent::BeginPlay()
{
	Super::BeginPlay();
}

void URangeAttackComponent::AttackCompleted()
{
	if (Target == nullptr)
	{
		return;
	}
	check(GetOwner()->IsA(AMoveableUnit::StaticClass()))
	StaticCast<AMoveableUnit*>(GetOwner())->Chase(Target, MaxRange);
}

void URangeAttackComponent::PerformAttack()
{
	ABaseProjectile* Arrow = GetWorld()->SpawnActorDeferred<ABaseProjectile>(ProjectileType, GetOwner()->GetTransform(), GetOwner(),GetWorld()->GetFirstPlayerController()->GetPawnOrSpectator(), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	if (Arrow)
	{
		Arrow->SetOwner(GetOwner());
		Arrow->SetDamage(Damage);
		FTransform ArrowRotation = Arrow->InitializeProjectile(StaticCast<ABaseUnit*>(GetOwner())->GetActorCenterLocation2d(), Target->GetActorLocation() + FVector(0,0, Target->GetActorHeight() / 2),  StaticCast<ABaseUnit*>(GetOwner())->GetActorHeight() / 2);
		UGameplayStatics::FinishSpawningActor(Arrow, ArrowRotation);
	}
	if (AMoveableUnit* MoveableOwner = Cast<AMoveableUnit>(GetOwner()))
	{
		MoveableOwner->StopChasing(Attacking);
		FTimerHandle StartChasingTimer;
		FTimerDelegate StartChasingDelegate;
		StartChasingDelegate.BindUFunction(this, "AttackCompleted");
		GetWorld()->GetTimerManager().SetTimer(StartChasingTimer, StartChasingDelegate, AttackLength, false);
	}
}

