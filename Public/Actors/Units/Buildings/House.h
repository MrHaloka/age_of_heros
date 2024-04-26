#pragma once

#include "CoreMinimal.h"
#include "Actors/Units/Buildings/BaseBuilding.h"
#include "House.generated.h"

/**
 * 
 */
UCLASS()
class AOE2_API AHouse : public ABaseBuilding
{
	GENERATED_BODY()
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
};
