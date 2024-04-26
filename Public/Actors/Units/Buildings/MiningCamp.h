#pragma once

#include "CoreMinimal.h"
#include "Actors/Units/Buildings/BaseBuilding.h"
#include "MiningCamp.generated.h"

class UDropoffComponent;
/**
 * 
 */
UCLASS()
class AOE2_API AMiningCamp : public ABaseBuilding
{
	GENERATED_BODY()
	virtual void BeginPlay() override;
protected:
	UPROPERTY()
	UDropoffComponent* StoneDropoffComponent;
	UPROPERTY()
	UDropoffComponent* GoldDropoffComponent;
};
