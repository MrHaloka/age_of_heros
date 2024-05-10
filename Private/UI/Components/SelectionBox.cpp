
#include "UI/Components/SelectionBox.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Image.h"
#include "UI/Slate/SSelectionBox.h"

USelectionBox::USelectionBox(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer),
	ColorAndOpacity(FLinearColor::White),
	LineThickness(10)
{
}

void USelectionBox::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	MySelectionBox.Reset();
}

void USelectionBox::EndSelection()
{
	MySelectionBox->SelectionEnded();
}

void USelectionBox::UpdateSelectionBox(const FVector2d& NewLocation)
{
	MySelectionBox->SetCurrentLocation(NewLocation);
}

void USelectionBox::SetColorAndOpacity(FLinearColor InColorAndOpacity)
{
	ColorAndOpacity = InColorAndOpacity;
}

const FLinearColor& USelectionBox::GetColorAndOpacity() const
{
	return ColorAndOpacity;
}

void USelectionBox::SetLineThickness(uint32 InLineThickness)
{
	LineThickness = InLineThickness;
}

uint32 USelectionBox::GetLineThickness() const
{
	return LineThickness;
}

TSharedRef<SWidget> USelectionBox::RebuildWidget()
{
	PRAGMA_DISABLE_DEPRECATION_WARNINGS
		MySelectionBox = SNew(SSelectionBox).LineColor(ColorAndOpacity).LineThickness(LineThickness);
	PRAGMA_ENABLE_DEPRECATION_WARNINGS
	return MySelectionBox.ToSharedRef();
}
