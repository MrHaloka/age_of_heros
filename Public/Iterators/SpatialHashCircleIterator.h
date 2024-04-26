#pragma once

struct FGridIndexer2d;

/**
 * Iterate in a circular manner, traversing all grids from the closest ring to the farthest.
 * This approach is more optimized when you need to find the closest point in grid in a large radius.
 */
class AOE2_API FSpatialHashCircleIterator
{
public:
	FSpatialHashCircleIterator(const FGridIndexer2d& Indexer, const FVector2d& QueryPoint, float Radius);
	TOptional<uint32> Iterate();
	bool IsFinish() const;
	uint8 GetCurrentRing() const;
protected:
	const FGridIndexer2d& Indexer;
	const uint32 CenterId;
	const uint8 MaxGridRadius;
	int32 RowDirection[4] = {1, 0, -1, 0};
	int32 ColumnDirection[4] = {0, 1, 0, 1};
	uint8 CurrentDirection;
	uint8 CurrentRing;
	uint8 CurrentSideStep;
	bool BEnded = false;
};
