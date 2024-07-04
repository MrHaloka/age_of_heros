#pragma once
#include "GamePlay/RTSGameMode.h"

struct FHillData;
class ALandscape;

/**
 * Spawns and stores a new landscape in the world with the given hills, scale, and material.
 */
class FLandscapeGenerator
{
public:
	FLandscapeGenerator(UWorld* InWorld, const TArray<FHillData>& Hills, const FVector& LandScapeScale, const TObjectPtr<UMaterialInterface>& LandscapeMaterial);
	int32 GetHeight(const FVector2d& LocationOnLandscape) const;
protected:
	void GenerateLandScape(const TArray<FHillData>& Hills, const FVector& LandScapeScale, const TObjectPtr<UMaterialInterface>& LandscapeMaterial);
protected:
	UWorld* World;
	ALandscape* Landscape;
};
