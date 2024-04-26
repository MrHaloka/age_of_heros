#pragma once
#include "CoreMinimal.h"
#include "Enums/Resource.h"
#include "SingleResource.generated.h"

USTRUCT(BlueprintType)
struct FSingleResource
{
	GENERATED_BODY()

public:
	FSingleResource(): Amount(0), Type(EResource::None){}
	FSingleResource(float Amount, EResource Type): Amount(Amount), Type(Type){}
	EResource GetType() const
	{
		return Type;
	}
	float GetAmount() 
	{
		return Amount;
	}
	float Amount;
	EResource Type;
};
