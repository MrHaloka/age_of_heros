#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataStructures/Resources.h"
#include "ResourcePanelWidget.generated.h"

/**
 * 
 */
class UTextBlock;
UCLASS()
class AOE2_API UResourcePanelWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UResourcePanelWidget(const FObjectInitializer& ObjectInitializer);
	void UpdateResourcesEvent(const FResources Resources);
	virtual void NativeConstruct() override;
protected:
	UPROPERTY()
	UTextBlock* Food;
	UPROPERTY()
	UTextBlock* Wood;
	UPROPERTY()
	UTextBlock* Gold;
	UPROPERTY()
	UTextBlock* Stone;
	UPROPERTY()
	UTextBlock* Population;
};
