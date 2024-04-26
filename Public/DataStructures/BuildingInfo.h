#pragma once
#include "Resources.h"
#include "Engine/DataTable.h"
#include "Actors/Units/BaseUnit.h"
#include "Actors/BaseHolograms.h"
#include "BuildingInfo.generated.h"

USTRUCT(BlueprintType)
struct FBuildingInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	FBuildingInfo() : SizeX(0), SizeY(0), SizeZ(0), MaxHP(0)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SizeX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SizeY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SizeZ;
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ABaseUnit> ConstructionClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ABaseHolograms> HologramClass;
	
	FResources GetPrice() const
	{
		return FResources(GoldPrice, StonePrice, WoodPrice, FoodPrice, PopulationPrice);
	}

	FVector2d GetSize() const
	{
		return FVector2d(SizeX, SizeY);
	}
};
