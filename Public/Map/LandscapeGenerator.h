#pragma once
#include "GamePlay/RTSGameMode.h"

struct FNoiseData;
struct FHillData;
class ALandscape;

/**
 * Spawns and stores a new landscape in the world with the given hills, scale, and material.
 */
class FLandscapeGenerator
{
public:
	FLandscapeGenerator(UWorld* InWorld, const FMapGenerator& InMapGenerator, const FNoiseData& NoiseData, const FNoiseData& SecondLayerNoise, const uint16 MaxHillHeightTile);
	int32 GetHeight(const FVector2d& LocationOnLandscape) const;
protected:
	void GenerateLandScape(const TArray<FHillData>& Hills, const FVector& LandScapeScale, const TObjectPtr<UMaterialInterface>& LandscapeMaterial);
	FHillData GenerateBaseLayer(float NoiseScale, float NoiseRange, uint16 MaxHillHeightTile, bool bIsBrush = false);
	void GenerateSecondLayer(FHillData& HillData, float NoiseScale, float NoiseRange, uint16 HillHeight);
protected:
	UWorld* World;
	ALandscape* Landscape;
	const FMapGenerator& MapGenerator;
};
