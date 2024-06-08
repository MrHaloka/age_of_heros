#pragma once

#include "CoreMinimal.h"
#include "Actors/Units/Buildings/BaseBuilding.h"
#include "Tower.generated.h"

class URangeAttackComponent;
class USightComponent;
UCLASS()
class AOE2_API ATower : public ABaseBuilding
{
	GENERATED_BODY()
	ATower();
	virtual void BeginPlay() override;
protected:
	UPROPERTY(EditDefaultsOnly)
	URangeAttackComponent* RangeAttackComponent;
	UPROPERTY(EditDefaultsOnly)
	USightComponent* SightComponent;
};
