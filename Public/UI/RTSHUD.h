#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Actors/Units/Villager.h"
#include "RTSHUD.generated.h"

class UUnitSlectionWidget;
class UMiniMapWidget;
class UVillagerPanel;
class UUnitPanelWidget;
class ABaseBuilding;
class UBuildingsPanelWidget;
class UResourcePanelWidget;
/**
 * 
 */
UCLASS()
class AOE2_API ARTSHUD : public AHUD
{
	GENERATED_BODY()
public:
	ARTSHUD();
	virtual void BeginPlay() override;
	void UpdateResources(const FResources Resources);
	void ShowTownCenterPanel(ABaseBuilding* BaseBuilding);
	void RemoveTheActivePanel() const;
	void ShowVillagerPanel(AVillager* Villager);
	UVillagerPanel* GetVillagerPanel();
	virtual void Tick(float DeltaSeconds) override;
	void UnitMoved(uint32 Id, const FVector2d& OldLocation, const FVector2d& NewLocation, const FColor& OldSpotColor);
	void UnitMoved(uint32 Id, const FVector2d& OldLocation, const FVector2d& NewLocation);
	void UnitAdded(const FVector2d& Location, const FColor& Color);
	void AddAlertAnimation(uint32 Id, const FVector2d& Location);
	void UpdateSelectionBox(const FVector2d& CursorLocation);
	void EndSelection();

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UResourcePanelWidget> ResourcePanelWidget;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UBuildingsPanelWidget> TownCenterPanelWidget;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UVillagerPanel> VillagerPanelWidget;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMiniMapWidget> MiniMapWidget;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUnitSlectionWidget> UnitSelectionWidget;
	UPROPERTY()
	UResourcePanelWidget* ResourcePanel;
	UPROPERTY()
	UBuildingsPanelWidget* TownCenterPanel;
	UPROPERTY()
	UVillagerPanel* VillagerPanel;
	UPROPERTY()
	UMiniMapWidget* MiniMap;
	UPROPERTY()
	UUnitSlectionWidget* UnitSelection;
	UPROPERTY()
	UUserWidget* ActiveWidget = nullptr;
};
