#pragma once

#include "CoreMinimal.h"
#include "Actors/Units/MoveableUnit.h"
#include "Scout.generated.h"

class UMeleeAttack;
class USightComponent;
UCLASS()
class AOE2_API AScout : public AMoveableUnit
{
	GENERATED_BODY()
public:
	AScout();
	virtual void BeginPlay() override;
protected:
	UPROPERTY(EditDefaultsOnly)
	UMeleeAttackComponent* MeleeAttackComponent;
	UPROPERTY(EditDefaultsOnly)
	USightComponent* SightComponent;
};
