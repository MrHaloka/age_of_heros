#pragma once
#include "CoreMinimal.h"

struct FNoiseData
{
	FNoiseData(float InNoiseScale, float InNoiseRange): NoiseScale(InNoiseScale), NoiseRange(InNoiseRange)
	{
		check(NoiseRange >= 0 && NoiseRange <= 2)
	}
	FNoiseData():NoiseScale(0), NoiseRange(0)
	{
	}
	float NoiseScale;
	float NoiseRange;
};
