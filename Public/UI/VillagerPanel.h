#pragma once

#include "CoreMinimal.h"
#include "UI/UnitPanelWidget.h"
#include "VillagerPanel.generated.h"

class FObjectsManager;
/**
 * 
 */
class UBorder;
UCLASS()
class AOE2_API UVillagerPanel : public UUnitPanelWidget
{
	GENERATED_BODY()
	
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
public:
	virtual void SetVisibility(ESlateVisibility InVisibility) override;
	void PlaceBuilding(const FVector2d& Location);
	void ResetBuilding();
protected:
	UPROPERTY(EditDefaultsOnly)
	UMaterial* Allowed;
	UPROPERTY(EditDefaultsOnly)
	UMaterial* NotAllowed;
	UFUNCTION()
	void ShowBuildPanel();
	UFUNCTION()
	void ShowBuilding(uint32 BuildingId);
	UFUNCTION()
	void ShowLumberCamp();
	UFUNCTION()
	void ShowHouse();
	UFUNCTION()
	void ShowMiningCamp();
	void ResetToDefault();
	UPROPERTY()
	UBorder* MainMenu;
	UPROPERTY()
	UBorder* BuildingMenu;
	EWidgetTickFrequency TickFrequency = EWidgetTickFrequency::Never;
	FVector2d LastMouseLocationOnGrid;
	void ChangeBuildingLocationBaseOnMouse();
};
