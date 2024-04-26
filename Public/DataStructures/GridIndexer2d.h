#pragma once
#include "MathUtil.h"

struct FGridIndexer2d
{
	const uint8 GridSize;
	const uint16 CellSize;
	const uint16 MaxID;
	FGridIndexer2d(uint8 GridSize, uint16 CellSize) : GridSize(GridSize), CellSize(CellSize), MaxID(GridSize * GridSize)
	{
		ensure(GridSize > 0);
		ensure(CellSize > 0);
	}

	uint32 ToGrid(const FVector2d& Location) const
	{
		return FMath::Clamp(TMathUtil<int>::Floor(Location.X / CellSize),0, GridSize-1) +
			FMath::Clamp(TMathUtil<int>::Floor(Location.Y / CellSize),0, GridSize-1) * GridSize;
	}

	FVector2d FromGrid(uint32 GridPoint) const
	{
		return FVector2d(GridPoint % GridSize,FMath::Floor(GridPoint / GridSize)) * CellSize;
	}

	TOptional<uint32> GetNextColumn(uint32 GridId, int8 Step = 1) const
	{
		if(Step != 0 && (GridId + Step) % GridSize == 0)
		{
			return NullOpt;
		}
		return GridId + Step;
	}

	TOptional<uint32> GetNextRow(uint32 GridId, int8 Step = 1) const
	{
		uint32 NextRow = GridId + GridSize * Step;
		if(NextRow > MaxID || NextRow < 0)
		{
			return NullOpt;
		}
		return NextRow;
	}
};
