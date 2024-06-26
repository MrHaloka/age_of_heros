﻿#include "Iterators/SpatialHashIterator.h"
#include "DataStructures/GridIndexer2d.h"

FSpatialHashIterator::FSpatialHashIterator(const FGridIndexer2d& Indexer, const FVector2d& QueryPoint, float Radius) :
Indexer(Indexer),
MinIndexID(Indexer.ToGrid(QueryPoint - Radius * FVector2d::One())),
MaxColumnDistance(Indexer.ToGrid(QueryPoint + Radius * FVector2d(1,0)) - Indexer.ToGrid(QueryPoint - Radius * FVector2d(1,0))),
MaxRowDistance((Indexer.ToGrid(QueryPoint + Radius * FVector2d(0,1)) - Indexer.ToGrid(QueryPoint - Radius * FVector2d(0,1))) / Indexer.GridSize),
RowCounter(0),
ColumnCounter(0)
{
}
FSpatialHashIterator::FSpatialHashIterator(const FGridIndexer2d& Indexer, const FVector2d& MinPoint, const FVector2d& MaxPoint):
Indexer(Indexer),
MinIndexID(Indexer.ToGrid(MinPoint)),
MaxColumnDistance(Indexer.ToGrid(FVector2d(MaxPoint.X, 0)) - Indexer.ToGrid(FVector2d(MinPoint.X,0))),
MaxRowDistance(Indexer.ToGrid(FVector2d(0, MaxPoint.Y)) - Indexer.ToGrid(FVector2d(0,MinPoint.Y))),
RowCounter(0),
ColumnCounter(0)
{
	
}

TOptional<uint32> FSpatialHashIterator::Iterate()
{
	TOptional<uint32> CurrentRow = Indexer.GetNextRow(MinIndexID, RowCounter);
	TOptional<uint32> Result = CurrentRow.IsSet()?  Indexer.GetNextColumn(CurrentRow.GetValue(), ColumnCounter) : NullOpt;
	ColumnCounter++;
	if (ColumnCounter > MaxColumnDistance)
	{
		RowCounter++;
		ColumnCounter = 0;
	}
	if (RowCounter > MaxRowDistance)
	{
		BEnded = true;
	}
	return Result;
}

bool FSpatialHashIterator::IsFinish() const
{
	return BEnded;
}

