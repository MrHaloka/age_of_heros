#pragma once
#include "CoreMinimal.h"
#include "NaturalResourcesData.h"
#include "NoiseData.h"
#include "PlayerBaseData.h"

struct FMapGenerateData
{
	FMapGenerateData(
		TArray<FNoiseData>&& InNoiseData,
		FNoiseData&& InSecondLayer,
		TArray<FResourceSpawnerData>&& InPlayersResourceData,
		FNaturalResourcesData&& InNaturalResourcesData,
		FPlayerBaseData&& InPlayerBaseData
	):
		NoiseData(MoveTemp(InNoiseData)),
		SecondLayer(MoveTemp(InSecondLayer)),
		PlayersResourceData(MoveTemp(InPlayersResourceData)),
		NaturalResourcesData(MoveTemp(InNaturalResourcesData)),
		PlayerBaseData(MoveTemp(InPlayerBaseData))
	{
	}

	FMapGenerateData()
	{
	}

	TArray<FNoiseData> NoiseData;
	FNoiseData SecondLayer;
	TArray<FResourceSpawnerData> PlayersResourceData;
	FNaturalResourcesData NaturalResourcesData;
	FPlayerBaseData PlayerBaseData;
};
