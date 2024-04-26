#pragma once
#include "Resources.h"
#include "Engine/DataTable.h"
#include "Actors/Units/BaseUnit.h"
#include "UnitInfo.generated.h"

USTRUCT(BlueprintType)
struct FUnitInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	FUnitInfo() : CollisionRadius(0), MaxHP(0), FoodPrice(0), WoodPrice(0), GoldPrice(0), StonePrice(0),
	              PopulationPrice(0),TimeToComplete(0)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CollisionRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 FoodPrice;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 WoodPrice;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 GoldPrice;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 StonePrice;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PopulationPrice;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TimeToComplete;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ABaseUnit> UnitClass;

	FResources GetPrice()
	{
		return FResources(GoldPrice, StonePrice, WoodPrice, FoodPrice, PopulationPrice);
	}
};
