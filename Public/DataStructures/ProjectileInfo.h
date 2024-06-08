#pragma once
#include "Engine/DataTable.h"
#include "ProjectileInfo.generated.h"

class ABaseProjectile;
USTRUCT(BlueprintType)
struct FProjectileInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	FProjectileInfo() :CollisionRadius(0), Speed(0)
	{
	}
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CollisionRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ABaseProjectile> ProjectileClass;
};
