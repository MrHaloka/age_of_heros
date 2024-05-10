#pragma once
#include "Styling/SlateColor.h"
#include "Widgets/SLeafWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "CoreMinimal.h"

class SSelectionBox : public SLeafWidget
{

public:
	SLATE_BEGIN_ARGS(SSelectionBox):
	_LineColor(FColor::White),
	_LineThickness(0)
	{}
	SLATE_ARGUMENT(FLinearColor, LineColor)
	SLATE_ARGUMENT(uint8, LineThickness)
	SLATE_END_ARGS()
	SSelectionBox();
	void Construct( const FArguments& InArgs );
public:
	virtual int32 OnPaint( const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled ) const override;
	virtual FVector2D ComputeDesiredSize(float) const override;
public:
	void SetCurrentLocation(const FVector2d& NewStartLocation);
	void SelectionEnded();
protected:
	FVector2d CurrentLocation = FVector2d::Zero();
	FVector2d StartLocation = FVector2d::Zero();
	FLinearColor LineColor;
	uint8 LineThickness;
	bool bStarted = false;
};
