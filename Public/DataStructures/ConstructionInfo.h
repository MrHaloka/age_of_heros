#pragma once
#include "CoreMinimal.h"
#include "ConstructionInfo.generated.h"

USTRUCT(BlueprintType)
struct FConstructionInfo
{
	GENERATED_USTRUCT_BODY()
public:
	FConstructionInfo() : WorkersCount(0), TimeToComplete(0){}
	FConstructionInfo(float TimeToComplete) : WorkersCount(0), TimeToComplete(TimeToComplete){}
	uint8 WorkersCount;
	float TimeToComplete;
	bool IsConstructionStarted = false;
};
