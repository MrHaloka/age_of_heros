
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UnitPanelWidget.generated.h"

class ABaseUnit;
/**
 * 
 */
UCLASS()
class AOE2_API UUnitPanelWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetActiveUnit(ABaseUnit* NewUnit);
protected:
	UPROPERTY()
	ABaseUnit* Unit;
};
