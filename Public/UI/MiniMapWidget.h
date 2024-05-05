#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MiniMapWidget.generated.h"

/**
 * 
 */
class UImage;
struct FGeometry;
struct FPointerEvent;

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
	void UnitMoved(uint32 Id, const FVector2d& OldLocation, const FVector2d& NewLocation, const FColor& OldSpotColor = FColor(59, 94, 53 ,255));
	void AddAlertAnimation(uint32 Id, const FVector2d& Location);
	UFUNCTION()
	FEventReply MiniMapClicked(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
	
protected:
	void UpdatePixel(const uint32& X, const uint32& Y, const FColor& Color);
	uint8 TextureSize;
	uint16 CellSize;
	TMap<uint32, TPair<FVector2d, FColor>> Animated;
	UFUNCTION()
	void RemoveAnimatedId(uint32 AnimatedId);
	UFUNCTION()
	void ResetPixel(uint32 AnimatedId);
};
