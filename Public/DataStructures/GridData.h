#pragma once
#include "CoreMinimal.h"
#include "GridData.generated.h"

USTRUCT(BlueprintType)
struct FGridData
{
 	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	int32 GridSize;
	UPROPERTY(EditAnywhere)
	int32 TileSize;
	UPROPERTY(EditAnywhere)
	float GridThickness;
	UPROPERTY(EditAnywhere)
	UMaterialInterface* GridMaterial;

	FGridData()
	{
		GridSize = 0;
		TileSize = 0;
		GridThickness = 0.0f;
		GridMaterial = nullptr;
	}
};
