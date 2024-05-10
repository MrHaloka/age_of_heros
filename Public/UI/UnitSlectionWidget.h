#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UnitSlectionWidget.generated.h"

class USelectionBox;
UCLASS()
class AOE2_API UUnitSlectionWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	void UpdateSelectionBox(const FVector2d& CursorLocation);
	void EndSelection();
protected:
	UPROPERTY()
	USelectionBox* SelectionBox;
};
