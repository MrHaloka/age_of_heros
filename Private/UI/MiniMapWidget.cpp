#include "UI/MiniMapWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Image.h"
#include "GamePlay/GameStatics.h"
#include "GamePlay/PlayerSpectatorPawn.h"
#include "GamePlay/RTSGameMode.h"
#include "Helpers/GridHelper.h"
#include "CoreMinimal.h"
#include "Layout/Geometry.h"
#include "Managers/ObjectsManager.h"

/**
 * Load the texture with the map size and minimap background.
 */
void UMiniMapWidget::NativeConstruct()
{
	Super::NativeConstruct();
	MiniMapImage = WidgetTree->FindWidget<UImage>(TEXT("MiniMap"));
	MiniMapImage->OnMouseButtonDownEvent.BindUFunction(this, "MiniMapClicked");
	TextureSize = GameStatics::GetRTSGameMode()->GetGridSize();
	CellSize = GameStatics::GetRTSGameMode()->GetCellSize();
	TArray<uint8> BackGroundBGRA;
	for (uint8 i = 0; i < TextureSize; i++)
	{
		for (uint8 j = 0; j < TextureSize; j++)
		{
			//For now, the background is always displayed as green.
			BackGroundBGRA.Append({53, 94, 59 ,255});
		}
	}
	MiniMapTexture = UTexture2D::CreateTransient(GameStatics::GetRTSGameMode()->GetGridSize(), GameStatics::GetRTSGameMode()->GetGridSize(), PF_B8G8R8A8);
	void* TextureData = MiniMapTexture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
	FMemory::Memcpy(TextureData, BackGroundBGRA.GetData(), BackGroundBGRA.Num());
	MiniMapTexture->GetPlatformData()->Mips[0].BulkData.Unlock();
	MiniMapTexture->UpdateResource();
	check(MiniMapImage != nullptr)
	MiniMapImage->SetBrushFromTexture(MiniMapTexture);
	GameStatics::GetRTSGameMode()->LoadDefaultActors(StaticCast<APlayerSpectatorPawn*>(GetOwningPlayerPawn())->GetRTSHUD());
}

FEventReply UMiniMapWidget::MiniMapClicked(FGeometry MyGeometry, const FPointerEvent& MouseEvent)
{
	const FVector2d LocalMouseLocation = MyGeometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition());
	GetOwningPlayerPawn()->SetActorLocation(FVector(TextureSize - LocalMouseLocation.Y + 1, LocalMouseLocation.X, 0) * CellSize);
	FEventReply Reply;
	Reply.NativeReply = FReply::Handled();
	return Reply;
}

void UMiniMapWidget::UnitAdded(const FVector2d& Location, const FColor& Color)
{
	const FVector2d GridLocation = GridHelper::GetGridFromVector(Location);
	UpdatePixel(GridLocation.X, GridLocation.Y, Color);
}

void UMiniMapWidget::UnitMoved(uint32 Id, const FVector2d& OldLocation, const FVector2d& NewLocation, const FColor& OldSpotColor)
{
	const FVector2d GridNewLocation = GridHelper::GetGridFromVector(NewLocation);
	const FVector2d GridOldLocation = GridHelper::GetGridFromVector(OldLocation);
	if (Animated.Contains(Id))
	{
		Animated.FindChecked(Id).Key = GridNewLocation;
		UpdatePixel(GridNewLocation.X, GridNewLocation.Y, Animated.FindChecked(Id).Value);
	}
	else
	{
		UpdatePixel(GridNewLocation.X, GridNewLocation.Y, FColor::Blue);
	}
	UpdatePixel(GridOldLocation.X, GridOldLocation.Y, OldSpotColor);
}

void UMiniMapWidget::AddAlertAnimation(uint32 Id, const FVector2d& Location)
{
	if (Animated.Contains(Id))
	{
		return;
	}
	const FVector2d GridLocation = GridHelper::GetGridFromVector(Location);
	Animated.Add(Id, TPairInitializer(GridLocation, FColor::White));
	UpdatePixel(GridLocation.X, GridLocation.Y, FColor::White);
	
	FTimerHandle RemoveAnimationTimer;
	FTimerDelegate RemoveAnimationDelegate;
	RemoveAnimationDelegate.BindUFunction(this, "RemoveAnimatedId", Id);
	GetWorld()->GetTimerManager().SetTimer(RemoveAnimationTimer, RemoveAnimationDelegate, 1, false);
	
	FTimerHandle ResetAnimationTimer;
	FTimerDelegate ResetAnimationDelegate;
	ResetAnimationDelegate.BindUFunction(this, "ResetPixel", Id);
	GetWorld()->GetTimerManager().SetTimer(ResetAnimationTimer, ResetAnimationDelegate, 0.5, false);
}

void UMiniMapWidget::UpdatePixel(const uint32& X, const uint32& Y, const FColor& Color)
{
	const uint32 Index =  (Y + (TextureSize-(X + 1)) * TextureSize) * 4;
	FTexture2DMipMap& Mip = MiniMapTexture->GetPlatformData()->Mips[0];
	uint8* TextureData = StaticCast<uint8*>(Mip.BulkData.Lock(LOCK_READ_WRITE));
	TextureData[Index] = Color.B;
	TextureData[Index + 1] = Color.G;
	TextureData[Index + 2] = Color.R;
	TextureData[Index + 3] = Color.A;
	Mip.BulkData.Unlock();
	MiniMapTexture->UpdateResource();
}

void UMiniMapWidget::ResetPixel(uint32 AnimatedId)
{
	if (!Animated.Contains(AnimatedId))
	{
		return;
	}
	const FVector2d GridLocation = Animated.FindChecked(AnimatedId).Key;
	Animated.FindChecked(AnimatedId).Value = FColor::Blue;
	UpdatePixel(GridLocation.X, GridLocation.Y, FColor::Blue);
}

void UMiniMapWidget::RemoveAnimatedId(uint32 AnimatedId)
{
	if (Animated.Contains(AnimatedId))
	{
		Animated.Remove(AnimatedId);
	}
}