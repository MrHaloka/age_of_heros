#pragma once

#include "CoreMinimal.h"
#include "Actors/Units/MoveableUnit.h"
#include "Archer.generated.h"
class URangeAttackComponent;
class USightComponent;
UCLASS()
class AOE2_API AArcher : public AMoveableUnit
{
	GENERATED_BODY()
public:
	AArcher();
	virtual void BeginPlay() override;
protected:
	UPROPERTY(EditDefaultsOnly)
	URangeAttackComponent* RangeAttackComponent;
	UPROPERTY(EditDefaultsOnly)
	USightComponent* SightComponent;
};
