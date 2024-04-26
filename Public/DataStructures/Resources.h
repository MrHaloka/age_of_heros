#pragma once

#include "SingleResource.h"
#include "Resources.generated.h"

USTRUCT(BlueprintType)
struct FResources
{
	GENERATED_BODY()
	float Gold;
	float Stone;
	float Wood;
	float Food;
	uint32 Population;

	FResources() : Gold(0), Stone(0), Wood(0), Food(0), Population(0)
	{
	}

	FResources(float NewGold, float NewStone, float NewWood, float NewFood, uint16 NewPopulation) :
		Gold(NewGold), Stone(NewStone), Wood(NewWood), Food(NewFood), Population(NewPopulation)
	{
	}

	void AddByType(EResource Type, float NewResource)
	{
		switch (Type)
		{
		case EResource::Food:
			Food += NewResource;
			break;
		case EResource::Wood:
			Wood += NewResource;
			break;
		case EResource::Gold:
			Gold += NewResource;
			break;
		case EResource::Stone:
			Stone += NewResource;
			break;
		case EResource::Population:
			Population += NewResource;
			break;
		default: ;
		}
	}

	FResources operator*(float Multiplier) const
	{
		FResources Result;
		Result.Food = this->Food * Multiplier;
		Result.Gold = this->Gold * Multiplier;
		Result.Stone = this->Stone * Multiplier;
		Result.Population = this->Population * Multiplier;
		Result.Wood = this->Wood * Multiplier;
		return Result;
	}

	FResources(EResource Type, float NewResource) : FResources()
	{
		AddByType(Type, NewResource);
	}

	FResources operator+(FResources const& NewResource) const
	{
		FResources Result;
		Result.Food = this->Food + NewResource.Food;
		Result.Gold = this->Gold + NewResource.Gold;
		Result.Stone = this->Stone + NewResource.Stone;
		Result.Population = this->Population + NewResource.Population;
		Result.Wood = this->Wood + NewResource.Wood;
		return Result;
	}

	void operator+=(FResources const& NewResource)
	{
		this->Food += NewResource.Food;
		this->Gold += NewResource.Gold;
		this->Stone += NewResource.Stone;
		this->Population += NewResource.Population;
		this->Wood += NewResource.Wood;
	}
	void operator+=(FSingleResource const& NewResource)
	{
		AddByType(NewResource.Type, NewResource.Amount);
	}
	void operator-=(FResources const& NewResource)
	{
		this->Food -= NewResource.Food;
		this->Gold -= NewResource.Gold;
		this->Stone -= NewResource.Stone;
		this->Population -= NewResource.Population;
		this->Wood -= NewResource.Wood;
	}

	bool operator>=(FResources const& NewResource) const
	{
		return this->Food >= NewResource.Food &&
			this->Gold >= NewResource.Gold &&
			this->Stone >= NewResource.Stone &&
			this->Population >= NewResource.Population &&
			this->Wood >= NewResource.Wood;
	}
};
