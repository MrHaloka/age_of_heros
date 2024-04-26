#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class AOE2_API GridHelper
{
public:
	GridHelper();
	~GridHelper();
	static FVector2d GetLocationOnGridCornerFromVector(const FVector2d& Vector2, uint16 CellSize = 100);
	static FVector2d GetGridFromVector(const FVector2d& Location, uint16 CellSize = 100);
	static FVector2d GetGridFromVector(const FVector& Location, uint16 CellSize = 100);
};
