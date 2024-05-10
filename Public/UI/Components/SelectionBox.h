#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Slate/SSelectionBox.h"
#include "SelectionBox.generated.h"

UCLASS()
class AOE2_API USelectionBox : public UWidget
{
	GENERATED_UCLASS_BODY()
public:
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	void EndSelection();
	void UpdateSelectionBox(const FVector2d& NewLocation);
public:
	UPROPERTY(EditAnywhere, Category=Appearance, meta=( sRGB="true") )
	FLinearColor ColorAndOpacity;
	void SetColorAndOpacity(FLinearColor InColorAndOpacity);
	const FLinearColor& GetColorAndOpacity() const;
	UPROPERTY(EditAnywhere, Category=Appearance)
	uint8 LineThickness;
	void SetLineThickness(uint32 InLineThickness);
	uint32 GetLineThickness() const;
protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	TSharedPtr<SSelectionBox> MySelectionBox;
	
};
