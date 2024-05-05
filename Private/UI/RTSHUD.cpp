#include "UI/RTSHUD.h"

#include "DataStructures/Resources.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ResourcePanelWidget.h"
#include "UI/BuildingsPanelWidget.h"
#include "UI/UnitPanelWidget.h"
#include "UI/VillagerPanel.h"
#include "UI/MiniMapWidget.h"

ARTSHUD::ARTSHUD()
{
}

void ARTSHUD::BeginPlay()
{
	Super::BeginPlay();
	ResourcePanel = CreateWidget<UResourcePanelWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0), ResourcePanelWidget);
	ResourcePanel->AddToViewport();
	TownCenterPanel = CreateWidget<UBuildingsPanelWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0), TownCenterPanelWidget);
	TownCenterPanel->SetVisibility(ESlateVisibility::Collapsed);
	TownCenterPanel->AddToViewport();
	
	VillagerPanel = CreateWidget<UVillagerPanel>(UGameplayStatics::GetPlayerController(GetWorld(), 0), VillagerPanelWidget);
	VillagerPanel->SetVisibility(ESlateVisibility::Collapsed);
	VillagerPanel->AddToViewport();

	MiniMap = CreateWidget<UMiniMapWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0), MiniMapWidget);
	MiniMap->AddToViewport();
}

void ARTSHUD::UpdateResources(const FResources Resources)
{
	if (IsValid(ResourcePanel))
	{
		ResourcePanel->UpdateResourcesEvent(Resources);
	}
}

void ARTSHUD::ShowTownCenterPanel(ABaseBuilding* BaseBuilding)
{
	RemoveTheActivePanel();
	TownCenterPanel->SetActiveBuilding(BaseBuilding);
	ActiveWidget = TownCenterPanel;
	TownCenterPanel->SetVisibility(ESlateVisibility::Visible);
}

void ARTSHUD::RemoveTheActivePanel() const
{
	if (ActiveWidget != nullptr)
	{
		ActiveWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}
UVillagerPanel* ARTSHUD::GetVillagerPanel()
{
	return VillagerPanel;
}

void ARTSHUD::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ARTSHUD::UnitMoved(uint32 Id, const FVector2d& OldLocation, const FVector2d& NewLocation, const FColor& OldSpotColor)
{
	MiniMap->UnitMoved(Id, OldLocation, NewLocation, OldSpotColor);
}

void ARTSHUD::UnitMoved(uint32 Id, const FVector2d& OldLocation, const FVector2d& NewLocation)
{
	MiniMap->UnitMoved(Id, OldLocation, NewLocation);
}

void ARTSHUD::UnitAdded(const FVector2d& Location, const FColor& Color)
{
	MiniMap->UnitAdded(Location, Color);
}

void ARTSHUD::AddAlertAnimation(uint32 Id, const FVector2d& Location)
{
	MiniMap->AddAlertAnimation(Id, Location);
}

void ARTSHUD::ShowVillagerPanel(AVillager* Villager)
{
	RemoveTheActivePanel();
	VillagerPanel->SetActiveUnit(Villager);
	ActiveWidget = VillagerPanel;
	VillagerPanel->SetVisibility(ESlateVisibility::Visible);
}
