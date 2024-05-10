#include "UI/UnitSlectionWidget.h"

#include "Blueprint/WidgetTree.h"
#include "UI/Components/SelectionBox.h"

void UUnitSlectionWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SelectionBox = WidgetTree->FindWidget<USelectionBox>(TEXT("UnitSelectionBox"));
}

void UUnitSlectionWidget::UpdateSelectionBox(const FVector2d& CursorLocation)
{
	SelectionBox->UpdateSelectionBox(CursorLocation);
}

void UUnitSlectionWidget::EndSelection()
{
	SelectionBox->EndSelection();
}
