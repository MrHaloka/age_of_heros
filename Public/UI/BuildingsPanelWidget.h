#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BuildingsPanelWidget.generated.h"

class ABaseBuilding;
/**
 * 
 */
UCLASS()
class AOE2_API UBuildingsPanelWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UBuildingsPanelWidget(const FObjectInitializer& ObjectInitializer);
	void SetActiveBuilding(ABaseBuilding* BaseBuilding);
protected:
	UPROPERTY()
	ABaseBuilding* CurrentBuilding = nullptr;
};
