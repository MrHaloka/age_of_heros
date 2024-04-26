#include "UI/BuildingsPanelWidget.h"

UBuildingsPanelWidget::UBuildingsPanelWidget(const FObjectInitializer& ObjectInitializer): UUserWidget(ObjectInitializer)
{
}

void UBuildingsPanelWidget::SetActiveBuilding(ABaseBuilding* BaseBuilding)
{
	CurrentBuilding = BaseBuilding;
}
