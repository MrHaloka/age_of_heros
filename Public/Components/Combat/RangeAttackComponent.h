#pragma once

#include "CoreMinimal.h"
#include "BaseCombatComponent.h"
#include "RangeAttackComponent.generated.h"


class ABaseProjectile;
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AOE2_API URangeAttackComponent : public UBaseCombatComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	void AttackCompleted();
	virtual void PerformAttack() override;
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<ABaseProjectile> ProjectileType;
};
