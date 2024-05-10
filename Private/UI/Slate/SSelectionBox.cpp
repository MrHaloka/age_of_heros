#include "UI/Slate/SSelectionBox.h"
#include "Rendering/DrawElements.h"

SSelectionBox::SSelectionBox()
{
	SetCanTick(false);
	bCanSupportFocus = false;
	bStarted = false;
}

void SSelectionBox::Construct(const FArguments& InArgs)
{
	LineThickness = InArgs._LineThickness;
	LineColor = InArgs._LineColor;
}

int32 SSelectionBox::OnPaint( const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled ) const
{
	// Check if drawing has actually started to prevent BP-Editor from attempting to draw a rectangle prematurely.
	if (bStarted)
	{
		FSlateDrawElement::MakeLines(OutDrawElements, LayerId,  AllottedGeometry.ToPaintGeometry(), {StartLocation, FVector2d(CurrentLocation.X, StartLocation.Y)}, ESlateDrawEffect::None, LineColor, true, LineThickness);
		FSlateDrawElement::MakeLines(OutDrawElements, LayerId,  AllottedGeometry.ToPaintGeometry(), {FVector2d(StartLocation.X, CurrentLocation.Y), CurrentLocation}, ESlateDrawEffect::None, LineColor, true, LineThickness);
		FSlateDrawElement::MakeLines(OutDrawElements, LayerId,  AllottedGeometry.ToPaintGeometry(), {StartLocation, FVector2d(StartLocation.X, CurrentLocation.Y)}, ESlateDrawEffect::None, LineColor, true, LineThickness);
		FSlateDrawElement::MakeLines(OutDrawElements, LayerId,  AllottedGeometry.ToPaintGeometry(), {CurrentLocation, FVector2d(CurrentLocation.X, StartLocation.Y)}, ESlateDrawEffect::None, LineColor, true, LineThickness);
	}
	return LayerId;
}

FVector2D SSelectionBox::ComputeDesiredSize(float) const
{
	return FVector2d::Zero();
}

void SSelectionBox::SetCurrentLocation(const FVector2d& NewStartLocation)
{
	if (!bStarted)
	{
		StartLocation = NewStartLocation * GetCachedGeometry().GetLocalSize();
		CurrentLocation = StartLocation;
		SetVisibility(EVisibility::Visible);
		bStarted = true;
	}
	else
	{
		CurrentLocation = NewStartLocation * GetCachedGeometry().GetLocalSize();
	}
}

void SSelectionBox::SelectionEnded()
{
	SetVisibility(EVisibility::Collapsed);
	bStarted = false;
}
