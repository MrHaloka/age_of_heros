#pragma once
#include "Resources.h"
#include "Engine/DataTable.h"
#include "Enums/UpgradeType.h"
#include "Upgrades.generated.h"
USTRUCT(BlueprintType)
struct FUpgrades : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EUpgradeType> Type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Amount;
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
	TSubclassOf<ABaseUnit> TargetUnit;
	
	FResources GetPrice()
	{
		return FResources(GoldPrice, StonePrice, WoodPrice, FoodPrice, PopulationPrice);
	}
};
