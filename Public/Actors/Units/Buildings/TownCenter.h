#pragma once

#include "CoreMinimal.h"
#include "Actors/Units/Buildings/BaseBuilding.h"
#include "TownCenter.generated.h"

/**
 * 
 */
UCLASS()
class AOE2_API ATownCenter : public ABaseBuilding
{
	GENERATED_BODY()
public:
	ATownCenter();
	virtual void ShowUnitPanel(ARTSHUD* TargetHUD) override;
};
