#pragma once

struct FGridIndexer2d;

class AOE2_API FSpatialHashIterator
{
public:
	FSpatialHashIterator(const FGridIndexer2d& Indexer, const FVector2d& QueryPoint, float Radius);
	FSpatialHashIterator(const FGridIndexer2d& Indexer, const FVector2d& MinPoint, const FVector2d& MaxPoint);
	TOptional<uint32> Iterate();
	bool IsFinish() const;

protected:
	const FGridIndexer2d& Indexer;
	const uint32 MinIndexID;
	const uint32 MaxColumnDistance;
	const uint32 MaxRowDistance;
	uint32 RowCounter;
	uint32 ColumnCounter;
	bool BEnded = false;
};
