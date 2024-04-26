#include "Helpers/GridHelper.h"

GridHelper::GridHelper()
{
}

GridHelper::~GridHelper()
{
}

FVector2d GridHelper::GetLocationOnGridCornerFromVector(const FVector2d& Location, uint16 CellSize)
{
	return FVector2d((FMath::Floor<int32>(Location.X / CellSize) * CellSize), (FMath::Floor<int32>(Location.Y / CellSize) * CellSize));
}

FVector2d GridHelper::GetGridFromVector(const FVector2d& Location, uint16 CellSize)
{
	return FVector2d(FMath::Floor<int32>(Location.X / CellSize), FMath::Floor<int32>(Location.Y / CellSize));
}

FVector2d GridHelper::GetGridFromVector(const FVector& Location, uint16 CellSize)
{
	return FVector2d((FMath::Floor(Location.X / CellSize)), (FMath::Floor(Location.Y / CellSize)));
}


