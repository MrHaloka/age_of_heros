#pragma once
#include "Engine/DataTable.h"
#include "Enums/Resource.h"
#include "Actors/Resources/BaseResources.h"
#include "ResourcesInfo.generated.h"

USTRUCT(BlueprintType)
struct FResourcesInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	FResourcesInfo() :Name(""),Type(EResource::None), PerSecond(0), MaxResource(0), MiniMapColor(FColor::White)
	{
	}
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EResource Type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PerSecond;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxResource;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ABaseResources> ResourceClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FColor MiniMapColor;
};
