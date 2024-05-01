#include "UI/MiniMapWidget.h"

#include "Actors/Resources/BaseResources.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Image.h"
#include "GamePlay/GameStatics.h"
#include "GamePlay/PlayerSpectatorPawn.h"
#include "GamePlay/RTSGameMode.h"
#include "Helpers/GridHelper.h"
#include "Managers/ObjectsManager.h"
/**
 * Load the texture with the map size and minimap background.
 */
void UMiniMapWidget::NativeConstruct()
{
	Super::NativeConstruct();
	MiniMapImage = WidgetTree->FindWidget<UImage>(TEXT("MiniMap"));
	TextureSize = GameStatics::GetRTSGameMode()->GetGridSize();
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

void UMiniMapWidget::UnitAdded(const FVector2d& Location, const FColor& Color)
{
	const FVector2d GridLocation = GridHelper::GetGridFromVector(Location);
	UpdatePixel(GridLocation.X, GridLocation.Y, Color);
}

void UMiniMapWidget::UnitMoved(const FVector2d& OldLocation, const FVector2d& NewLocation, const FColor& OldSpotColor)
{
	const FVector2d GridNewLocation = GridHelper::GetGridFromVector(NewLocation);
	const FVector2d GridOldLocation = GridHelper::GetGridFromVector(OldLocation);
	UpdatePixel(GridNewLocation.X, GridNewLocation.Y, FColor(10,0, 200,255));
	UpdatePixel(GridOldLocation.X, GridOldLocation.Y, OldSpotColor);
}

void UMiniMapWidget::UpdatePixel(const uint32& X, const uint32& Y,const FColor& Color)
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

void UMiniMapWidget::ResetPixel(const uint32& X, const uint32& Y)
{
	const uint32 Index =  (Y + (TextureSize-(X + 1)) * TextureSize) * 4;
	FTexture2DMipMap& Mip = MiniMapTexture->GetPlatformData()->Mips[0];
	uint8* TextureData = StaticCast<uint8*>(Mip.BulkData.Lock(LOCK_READ_WRITE));
	TextureData[Index] = 53;
	TextureData[Index + 1] = 94;
	TextureData[Index + 2] = 59;
	TextureData[Index + 3] = 255;
	Mip.BulkData.Unlock();
	MiniMapTexture->UpdateResource();
}
