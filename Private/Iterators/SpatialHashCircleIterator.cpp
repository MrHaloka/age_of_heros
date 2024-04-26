#include "Iterators/SpatialHashCircleIterator.h"

#include "DataStructures/GridIndexer2d.h"

FSpatialHashCircleIterator::FSpatialHashCircleIterator(const FGridIndexer2d& Indexer, const FVector2d& QueryPoint, float Radius):
Indexer(Indexer),
CenterId(Indexer.ToGrid(QueryPoint)),
MaxGridRadius(TMathUtil<int>::Floor(Radius))
{
	CurrentRing = 0;
	CurrentDirection = 3;
	CurrentSideStep = 0;
}

TOptional<uint32> FSpatialHashCircleIterator::Iterate()
{
	TOptional<uint32> Result = NullOpt;
	if (ColumnDirection[CurrentDirection] == 0)
	{
		TOptional<uint32> CurrentRowId = Indexer.GetNextRow(CenterId, RowDirection[CurrentDirection] * CurrentRing);
		if (CurrentRowId.IsSet())
		{
			Result = Indexer.GetNextColumn(CurrentRowId.GetValue(), RowDirection[CurrentDirection] * CurrentSideStep);
			CurrentSideStep++;
		}
	}
	else
	{
		TOptional<uint32> CurrentColumnId = Indexer.GetNextColumn(CenterId, ColumnDirection[CurrentDirection] * CurrentRing);
		if (CurrentColumnId.IsSet())
		{
			Result = Indexer.GetNextColumn(CurrentColumnId.GetValue(), -ColumnDirection[CurrentDirection] * CurrentSideStep);
			CurrentSideStep++;
		}
	}
	
	if (CurrentSideStep >= CurrentRing)
	{
		CurrentDirection++;
		CurrentSideStep = 1;
	}
	if (CurrentDirection == 4)
	{
		CurrentDirection = 0;
		CurrentRing++;
		if (CurrentRing > MaxGridRadius)
		{
			BEnded = true;
		}
	}
	return Result;
}

bool FSpatialHashCircleIterator::IsFinish() const
{
	return BEnded;
}

uint8 FSpatialHashCircleIterator::GetCurrentRing() const
{
	return CurrentRing;
}
