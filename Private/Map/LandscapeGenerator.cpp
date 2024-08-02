#include "Map/LandscapeGenerator.h"
#include "Landscape.h"
#include "DataStructures/MapGeneration/HillData.h"
#include "DataStructures/MapGeneration/LandScapeData.h"
#include "DataStructures/MapGeneration/NoiseData.h"
#include "GamePlay/GameStatics.h"
#include "Managers/ObjectsManager.h"
#include "Map/MapGenerator.h"
#include "Map/PlayersBaseSpawner.h"

FLandscapeGenerator::FLandscapeGenerator(UWorld* InWorld, const FMapGenerator& InMapGenerator, const FNoiseData& NoiseData, const FNoiseData& SecondLayerNoise, const uint16 MaxHillHeightTile):
	World(InWorld), MapGenerator(InMapGenerator)
{
	check(World != nullptr)
	TArray<FHillData> Hills;
	FHillData BaseLayer = GenerateBaseLayer(NoiseData.NoiseScale, NoiseData.NoiseRange, MaxHillHeightTile);
	GenerateSecondLayer(BaseLayer, SecondLayerNoise.NoiseScale, SecondLayerNoise.NoiseRange, MaxHillHeightTile);
	Hills.Add(BaseLayer);
	FVector LandscapeScale = FVector(GameStatics::GetRTSGameMode()->GetCellSize(), GameStatics::GetRTSGameMode()->GetCellSize(),  GameStatics::GetRTSGameMode()->GetZCellHeight());
	GenerateLandScape(Hills, LandscapeScale, MapGenerator.GetLandscapeMaterial());
}

/**
 * Generates hill data for the landscape.
 * @param NoiseScale The Perlin noise scale.
 * @param NoiseRange The range between 0 and 2 within which hills are painted.
 * @param MaxHillHeightTile The maximum height of the hills.
 * @param bIsBrush Indicates whether this function should use a brush for drawing hills.
 * @return An FHillData object containing all the hill data related to this landscape.
 */
FHillData FLandscapeGenerator::GenerateBaseLayer(float NoiseScale, float NoiseRange, uint16 MaxHillHeightTile, bool bIsBrush)
{
	FHillData BaseLayer;
	const float MinAcceptableNoise = 1 - NoiseRange;
	for (int32 CurrentX = 0; CurrentX <= MapGenerator.GetMaxLocation().X; CurrentX += MapGenerator.GetObjectsManagerRef().GetCellSize())
	{
		for (int32 CurrentY = 0; CurrentY <=  MapGenerator.GetMaxLocation().Y; CurrentY += MapGenerator.GetObjectsManagerRef().GetCellSize())
		{
			const FVector2d CurrentLocation = FVector2d(CurrentX, CurrentY);
			if (MapGenerator.GetPlayerBaseSpawner()->IsInPlayerSafeArea(CurrentLocation))
			{
				continue;
			}
			const float CurrentNoise = FMath::PerlinNoise2D((GridHelper::GetGridFromVector(CurrentLocation) + MapGenerator.GetRandomSeed()) * NoiseScale);
			if (CurrentNoise < MinAcceptableNoise)
			{
				continue;
			}
			const uint16 Height = FMath::CeilToInt(( 1 - ((1 - CurrentNoise) / NoiseRange)) * MaxHillHeightTile);
			BaseLayer.PaintNewHill(CurrentLocation, Height, bIsBrush);
		}
	}
	return BaseLayer;
}

/**
 * Creates a second layer of noise on top of the base layer to give it a more natural look.
 * @param HillData The base layer generated data. This method generates noise to add to this data.
 * @param NoiseScale The Perlin noise scale.
 * @param NoiseRange The range between 0 and 2 within which hills are painted.
 * @param HillHeight The maximum height of the hills.
 */
void FLandscapeGenerator::GenerateSecondLayer(FHillData& HillData, float NoiseScale, float NoiseRange, uint16 HillHeight)
{
	const float SecondLayerSeed = MapGenerator.GetRandomSeed() + 500;
	const float MinAcceptableNoise = 1 - NoiseRange;
	for (const FVector2d& GridLocation : HillData.AddedGrids.Array())
	{
		const float CurrentNoise = FMath::PerlinNoise2D((GridHelper::GetGridFromVector(GridLocation) + SecondLayerSeed) * NoiseScale);
		if (CurrentNoise < MinAcceptableNoise)
		{
			continue;
		}
		const float SecondLayer = (1 - ((1 - CurrentNoise) / NoiseRange)) * HillHeight;
		HillData.SetHeightValueForGrid(GridLocation, SecondLayer, 16, 1);
	}
}

/**
 * Creates, spawns, and stores the landscape with the given data.
 * @param Hills The array of HillData used to generate hills on the landscape.
 * @param LandScapeScale The 3D scale of the landscape.
 * @param LandscapeMaterial The default material of the landscape.
 * TODO:: Landscape data needs to be imported from the map script.
 */
void FLandscapeGenerator::GenerateLandScape(const TArray<FHillData>& Hills, const FVector& LandScapeScale, const TObjectPtr<UMaterialInterface>& LandscapeMaterial)
{
	FLandScapeData LandScapeData(Hills, 63, 1, 2, 2);
	
	Landscape = World->SpawnActor<ALandscape>(ALandscape::StaticClass(), FTransform::Identity);
	Landscape->bCanHaveLayersContent = false;
	Landscape->LandscapeMaterial = LandscapeMaterial;
	Landscape->SetActorScale3D(LandScapeScale);
	Landscape->Import(FGuid::NewGuid(),
		0,
		0,
		LandScapeData.SizeX - 1,
		LandScapeData.SizeY - 1,
		LandScapeData.SectionsPerComponent,
		LandScapeData.QuadsPerComponent,
		LandScapeData.HeightDataPerLayers,
		nullptr,
		LandScapeData.MaterialLayerDataPerLayers,
		ELandscapeImportAlphamapType::Additive
		);
	Landscape->StaticLightingLOD = FMath::DivideAndRoundUp(FMath::CeilLogTwo((LandScapeData.SizeX * LandScapeData.SizeY) / (2048 * 2048) + 1), StaticCast<uint32>(2));
	ULandscapeInfo* LandscapeInfo = Landscape->GetLandscapeInfo();
	LandscapeInfo->UpdateLayerInfoMap(Landscape);
	Landscape->RegisterAllComponents();
	FPropertyChangedEvent MaterialPropertyChangedEvent(FindFieldChecked<FProperty>(Landscape->GetClass(), FName("LandscapeMaterial")));
	Landscape->PostEditChangeProperty(MaterialPropertyChangedEvent);
	Landscape->PostEditChange();
}

/**
 * Calculates the height of the 2D location on the hills.
 * @param LocationOnLandscape The 2D location on the landscape.
 * @return The height of the given location (0 if it's outside the landscape).
 */
int32 FLandscapeGenerator::GetHeight(const FVector2d& LocationOnLandscape) const
{
	TOptional<float> Result = Landscape->GetHeightAtLocation(FVector(LocationOnLandscape, 0));
	return Result.IsSet() ? Result.GetValue() : 0;
}