#pragma once
#include "CoreMinimal.h"
#include "GamePlay/GameStatics.h"

struct FResourcesInfo;

struct FResourceSpawnerData
{
	FResourceSpawnerData(): ResourcesInfo(nullptr), NumberOfResources(0), NumberOfResourcesTile(0), MinDistanceToPlayer(0), MaxDistanceToPlayer(0)
	{
	}

	FResourceSpawnerData(int32 InResourceId, int32 InNumberOfResources, int32 InNumberOfResourcesTile, int32 InMinDistanceToPlayer, int32 InMaxDistanceToPlayer):
		ResourcesInfo(GameStatics::GetResourceByID(InResourceId)),
		NumberOfResources(InNumberOfResources),
		NumberOfResourcesTile(InNumberOfResourcesTile),
		MinDistanceToPlayer(InMinDistanceToPlayer),
		MaxDistanceToPlayer(InMaxDistanceToPlayer)
	{
	}

	FResourceSpawnerData(const FResourcesInfo* InResourcesInfo, int32 InNumberOfResources, int32 InNumberOfResourcesTile, int32 InMinDistanceToPlayer, int32 InMaxDistanceToPlayer):
	ResourcesInfo(InResourcesInfo),
	NumberOfResources(InNumberOfResources),
	NumberOfResourcesTile(InNumberOfResourcesTile),
	MinDistanceToPlayer(InMinDistanceToPlayer),
	MaxDistanceToPlayer(InMaxDistanceToPlayer)
	{
	}
	
	const FResourcesInfo* ResourcesInfo;
	int32 NumberOfResources;
	int32 NumberOfResourcesTile;
	int32 MinDistanceToPlayer;
	int32 MaxDistanceToPlayer;
};
