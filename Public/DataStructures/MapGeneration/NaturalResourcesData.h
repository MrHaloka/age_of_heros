#pragma once
#include "CoreMinimal.h"
#include "ResourceSpawnerData.h"

struct FNaturalResourcesData
{
	FNaturalResourcesData(TArray<FResourceSpawnerData>&& InNaturalResources, int32 InForestPartition, int32 InMinForestTiles, int32 InMaxForestTiles):
		NaturalResources(MoveTemp(InNaturalResources)), ForestPartition(InForestPartition), MinForestTiles(InMinForestTiles), MaxForestTiles(InMaxForestTiles)
	{
	}
	
	FNaturalResourcesData(): ForestPartition(0), MinForestTiles(0), MaxForestTiles(0)
	{
	}
	
	TArray<FResourceSpawnerData> NaturalResources;
	int32 ForestPartition;
	int32 MinForestTiles;
	int32 MaxForestTiles;

};
