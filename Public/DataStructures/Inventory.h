#pragma once
#include "CoreMinimal.h"
#include "IPropertyTable.h"
#include "SingleResource.h"
#include "Enums/Resource.h"
#include "Inventory.generated.h"

USTRUCT(BlueprintType)
struct FInventory
{
	GENERATED_BODY()

public:
	FInventory():ResourceAmount(0), ResourceType(EResource::None), MaxSize(0){}
	FInventory(double MaxSize) :ResourceAmount(0), ResourceType(EResource::None), MaxSize(MaxSize)
	{
	}

	void operator+=(FSingleResource SingleResource)
	{
		UE_LOG(LogTemp, Warning, TEXT("Adding called"));
		if (ResourceType != SingleResource.Type)
		{
			ResourceAmount = FMath::Clamp<float>(SingleResource.Amount, 0, MaxSize);
			ResourceType = SingleResource.Type;
			return;
		}
		ResourceAmount = FMath::Clamp<float>(ResourceAmount + SingleResource.Amount, 0, MaxSize);
		UE_LOG(LogTemp, Warning, TEXT("added %f"), ResourceAmount);
	}

	FSingleResource GetData() const
	{
		return FSingleResource(ResourceAmount, ResourceType);
	}

	void Reset()
	{
		ResourceAmount = 0;
		ResourceType = EResource::None;
	}

	bool IsFull() const
	{
		return ResourceAmount >= MaxSize;
	}
	double ResourceAmount;
	EResource ResourceType;
	double MaxSize;
};
