#pragma once

#include "CoreMinimal.h"
#include "Actors/Units/Buildings/BaseBuilding.h"
#include "LumberCamp.generated.h"

class UDropoffComponent;
/**
 * 
 */
UCLASS()
class AOE2_API ALumberCamp : public ABaseBuilding
{
	GENERATED_BODY()
public:
	ALumberCamp();
	virtual void BeginPlay() override;
protected:
	UPROPERTY()
	UDropoffComponent* WoodDropoffComponent;
};
