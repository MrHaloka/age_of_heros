#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MiniMapWidget.generated.h"

/**
 * 
 */
class UImage;
UCLASS()
class AOE2_API UMiniMapWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	UPROPERTY()
	UTexture2D* MiniMapTexture;
	UPROPERTY()
	UImage* MiniMapImage;
	void UnitAdded(const FVector2d& Location, const FColor& Color);
	void UnitMoved(const FVector2d& OldLocation, const FVector2d& NewLocation, const FColor& OldSpotColor = FColor(59, 94, 53 ,255));
protected:
	void UpdatePixel(const uint32& X, const uint32& Y, const FColor& Colo);
	void ResetPixel(const uint32& X, const uint32& Y);
	uint32 TextureSize;
};
