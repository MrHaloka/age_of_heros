
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseCombatComponent.generated.h"

enum UnitStance : int;
enum EUnitState : int;
class ABaseUnit;
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AOE2_API UBaseCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBaseCombatComponent();
	UFUNCTION()
	virtual void SensedNewUnitListener(ABaseUnit* Unit);
	virtual void StartAttack(ABaseUnit* EnemyUnit);
	UFUNCTION()
	virtual void Attack();
	UFUNCTION()
	virtual void ResetTarget(AActor* DestroyedActor, EEndPlayReason::Type EndPlayReason);
	virtual void ResetTarget();
	virtual float GetReloadTime();
	virtual float GetCooldown();
	UFUNCTION()
	void OwnerPrepareToStateChangeListener(TEnumAsByte<EUnitState> NewState, TEnumAsByte<EUnitState> OldState);
	UFUNCTION()
	void ReachedTargetListener();
protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	virtual void ResetReload();
	void FindAndAttackTarget();
	virtual void PerformAttack();
	EUnitState GetOwnerState();
	void SetOwnerState(EUnitState UnitState);
	void SetTarget(ABaseUnit* NewTarget);
	void BuildingStartAttack(ABaseUnit* EnemyUnit);
	void MoveableStartAttack(ABaseUnit* EnemyUnit);
protected:
	UPROPERTY()
	ABaseUnit* Target;
	bool bIsOnReload = false;
	UPROPERTY(EditDefaultsOnly, Category = "Combat|Range")
	int32 MaxRange;
	UPROPERTY(EditDefaultsOnly, Category = "Combat|Range")
	int32 MaxPursuitRange;
	UPROPERTY(EditDefaultsOnly, Category = "Combat|AttackDetails")
	float ReloadTime;
	UPROPERTY(EditDefaultsOnly, Category = "Combat|AttackDetails")
	float Cooldown;
	UPROPERTY(EditDefaultsOnly, Category = "Combat|AttackDetails")
	float Damage;
	UPROPERTY(EditDefaultsOnly, Category = "Combat|AttackDetails")
	float AttackLength;
};
