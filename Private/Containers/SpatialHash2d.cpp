/**
 * This class includes functionalities inspired by UE::Geometry::TPointHashGrid2 , extending its capabilities
 * to suit specific requirements.
 */
#include "Containers/SpatialHash2d.h"
#include "Iterators/SpatialHashIterator.h"

FSpatialHash2d::FSpatialHash2d(const uint8 GridSize, const uint16 CellSize) : Indexer(FGridIndexer2d(GridSize, CellSize))
{
}

/**
 * Insert at given position. This function is thread-safe.
 * @param PointID the point's id to insert
 * @param Location the location associated with this PointID
 */
void FSpatialHash2d::InsertPoint(const uint32 PointID, const FVector2d& Location)
{
	uint32 IndexID = Indexer.ToGrid(Location);
	{
		FScopeLock Lock(&CriticalSection);
		Hash.Add(IndexID, PointID);
	}
}

/**
 * Insert at given positions. This function is thread-safe.
 * @param PointID the point's id to insert
 * @param Location the location associated with this PointID
 * @param UnitSize the size of the object
 */
void FSpatialHash2d::InsertPoints(const uint32 PointID, const FVector2d& Location, const FVector2d& UnitSize)
{
	for (int RowCounter = 0; RowCounter < UnitSize.X; RowCounter += 100)
	{
		for (int ColumnCounter = 0; ColumnCounter < UnitSize.Y; ColumnCounter += 100)
		{
			InsertPoint(PointID, Location + FVector2d(RowCounter, ColumnCounter));
		}
	}
}

/**
 * Insert at given position, without locking / thread-safety
 * @param PointID the point's id to insert
 * @param Location the location associated with this PointID
 */
void FSpatialHash2d::InsertPointUnsafe(const uint32 PointID, const FVector2d& Location)
{
	uint32 IndexID = Indexer.ToGrid(Location);
	Hash.Add(IndexID, PointID);
}

/**
 * Remove at given position. This function is thread-safe.
 * @param PointID the point's id to insert
 * @param Location the location associated with this PointID
 * @return true if the value existed at this position
 */
bool FSpatialHash2d::RemovePoint(const uint32 PointID, const FVector2d& Location)
{
	uint32 IndexID = Indexer.ToGrid(Location);
	{
		FScopeLock Lock(&CriticalSection);
		return Hash.RemoveSingle(IndexID, PointID) > 0;
	}
}

/**
 * Remove at given position, without locking / thread-safety
 * @param PointID the point's id to insert
 * @param Location the location associated with this PointID
 * @return true if the value existed at this position
 */
bool FSpatialHash2d::RemovePointUnsafe(const uint32 PointID, const FVector2d& Location)
{
	uint32 IndexID = Indexer.ToGrid(Location);
	return Hash.RemoveSingle(IndexID, PointID) > 0;
}

void FSpatialHash2d::RemovePointsUnsafe(const uint32 PointID, const FVector2d& Location, const FVector2d& UnitSize)
{
	for (int RowCounter = 0; RowCounter < UnitSize.X; RowCounter += 100)
	{
		for (int ColumnCounter = 0; ColumnCounter < UnitSize.Y; ColumnCounter += 100)
		{
			RemovePointUnsafe(PointID, Location + FVector2d(RowCounter, ColumnCounter));
		}
	}
}

/**
 * Move value from old to new position. This function is thread-safe.
 * @param PointID the point's id to insert
 * @param OldLocation the old location associated with this point's id
 * @param CurrentLocation the current location for this point
 */
bool FSpatialHash2d::UpdatePoint(const uint32 PointID, const FVector2d& OldLocation, const FVector2d& CurrentLocation)
{
	uint32 OldIndexID = Indexer.ToGrid(OldLocation);
	uint32 CurrentIndexID = Indexer.ToGrid(CurrentLocation);
	if (OldIndexID == CurrentIndexID)
	{
		return false;
	}
	bool BWasAtOldGrid;
	{
		FScopeLock Lock(&CriticalSection);
		BWasAtOldGrid = (Hash.RemoveSingle(OldIndexID, PointID) != 0);
	}
	check(BWasAtOldGrid);
	{
		FScopeLock Lock(&CriticalSection);
		Hash.Add(CurrentIndexID, PointID);
	}
	return true;
}

/**
 * Move value from old to new position, without locking / thread-safety
 * @param PointID the point's id to insert
 * @param OldLocation the old location associated with this point's id
 * @param CurrentLocation the current location for this point
 */
void FSpatialHash2d::UpdatePointUnsafe(const uint32 PointID, const FVector2d& OldLocation, const FVector2d& CurrentLocation)
{
	uint32 OldIndexID = Indexer.ToGrid(OldLocation);
	uint32 CurrentIndexID = Indexer.ToGrid(CurrentLocation);
	if (OldIndexID == CurrentIndexID)
	{
		return;
	}
	bool BWasAtOldGrid = Hash.RemoveSingle(OldIndexID, PointID) != 0;
	check(BWasAtOldGrid);
	Hash.Add(CurrentIndexID, PointID);
}

/**
 * Find nearest point in grid, within a given sphere, without locking / thread-safety.
 * @param QueryPoint the center of the query sphere
 * @param Radius the radius of the query sphere
 * @param DistanceSqFunc Function you provide which measures the distance square between QueryPoint and a Value
 * @param IgnoreFunc optional Function you may provide which will result in a Value being ignored if IgnoreFunc(Value) returns true
 * @return the found pair (Value,DistanceFunc(Value)), or (InvalidValue,MaxDouble) if not found
 */
TOptional<uint32> FSpatialHash2d::FindNearestInRadius(
	const FVector2d& QueryPoint,
	float Radius,
	TFunctionRef<float(uint32)> DistanceSqFunc,
	TFunctionRef<bool(uint32)> IgnoreFunc) const
{
	
	float MinDistSquare = TNumericLimits<float>::Max();
	TOptional<uint32> NearestID = NullOpt;
	float RadiusSquared = Radius * Radius;
	TArray<uint32> Values;
	FSpatialHashIterator SpatialHashIterator(Indexer, QueryPoint, Radius);
	while (!SpatialHashIterator.IsFinish())
	{
		TOptional<uint32> IndexId = SpatialHashIterator.Iterate();
		if (!IndexId.IsSet())
		{
			continue;
		}
		Values.Reset();
		Hash.MultiFind(IndexId.GetValue(), Values);
		for (uint32 Value : Values)
		{
			if (IgnoreFunc(Value))
			{
				continue;
			}
			float DistSquare = DistanceSqFunc(Value);
			if (DistSquare < RadiusSquared && DistSquare < MinDistSquare)
			{
				NearestID = Value;
				MinDistSquare = DistSquare;
			}
		}
	}
	return NearestID;
}

/**
 * Find first point within a given sphere, without locking / thread-safety.
 * @param QueryPoint the center of the query sphere
 * @param Radius the radius of the query sphere
 * @param DistanceSqFunc Function you provide which measures the distance square between QueryPoint and a Value
 * @param IgnoreFunc optional Function you may provide which will result in a Value being ignored if IgnoreFunc(Value) returns true
 * @return true as soon as find any point in grid
 */
bool FSpatialHash2d::IsAnyInRadius(
	const FVector2d& QueryPoint,
	float Radius,
	TFunctionRef<float(uint32)> DistanceSqFunc,
	TFunctionRef<bool(uint32)> IgnoreFunc) const
{
	float MinDistSquare = TNumericLimits<float>::Max();
	TOptional<uint32> NearestID = NullOpt;
	float RadiusSquared = Radius * Radius;
	TArray<uint32> Values;
	FSpatialHashIterator SpatialHashIterator(Indexer, QueryPoint, Radius);
	while (!SpatialHashIterator.IsFinish())
	{
		TOptional<uint32> IndexId = SpatialHashIterator.Iterate();
		if (!IndexId.IsSet())
		{
			continue;
		}
		Values.Reset();
		Hash.MultiFind(IndexId.GetValue(), Values);
		for (uint32 Value : Values)
		{
			if (IgnoreFunc(Value))
			{
				continue;
			}
			float DistSquare = DistanceSqFunc(Value);
			if (DistSquare < RadiusSquared && DistSquare < MinDistSquare)
			{
				return true;
			}
		}
	}
	return false;
}

/**
 * Find if there's any point within a given sphere, without locking / thread-safety.
 * @param QueryPoint the center of the query sphere
 * @param Radius the radius of the query sphere
 * @param DistanceSqFunc Function you provide which measures the distance between QueryPoint and a Value
 * @param IgnoreFunc optional Function you may provide which will result in a Value being ignored if IgnoreFunc(Value) returns true
 * @return a set contains all the points that were found in the radius
 */
TSet<uint32> FSpatialHash2d::FindAllInRadius(
	const FVector2d& QueryPoint,
	float Radius,
	TFunctionRef<float(const uint32&)> DistanceSqFunc,
	TFunctionRef<bool(const uint32&)> IgnoreFunc ) const
{
	TSet<uint32> Results;
	float RadiusSquared = Radius * Radius;
	TArray<uint32> Values;
	FSpatialHashIterator SpatialHashIterator(Indexer, QueryPoint, Radius);
	while (!SpatialHashIterator.IsFinish())
	{
		TOptional<uint32> IndexId = SpatialHashIterator.Iterate();
		if (!IndexId.IsSet())
		{
			continue;
		}
		Values.Reset();
		Hash.MultiFind(IndexId.GetValue(), Values);
		for (uint32 Value : Values)
		{
			if (IgnoreFunc(Value))
			{
				continue;
			}
			if (DistanceSqFunc(Value) < RadiusSquared)
			{
				Results.Add(Value);
			}
		}
	}
	return Results;
}

/**
 * Find if there's any point within a grid, without locking / thread-safety.
 * It get the gridId from indexer and call @link GetPointInGrid(uint32, TFunctionRef<bool(uint32)>)
 * @param GridLocation the grid location that you want to check
 * @param FilterFunction Function you provide which filter and check if there's any point inside
 * @return the first point or NullOpt if not found any
 */
TOptional<uint32> FSpatialHash2d::GetPointInGrid(const FVector2d& GridLocation, TFunctionRef<bool(uint32)> FilterFunction)
{
	uint32 GridId = Indexer.ToGrid(GridLocation);
	return GetPointInGrid(GridId, FilterFunction);
}

/**
 * Find if there's any point within a grid, without locking / thread-safety.
 * @param GridId the grid ID that you want to check
 * @param FilterFunction Function you provide which filter and check if there's any point inside
 * @return the first point or NullOpt if not found any
 */
TOptional<uint32> FSpatialHash2d::GetPointInGrid(uint32 GridId, TFunctionRef<bool(uint32)> FilterFunction)
{
	TArray<uint32> Values;
	Hash.MultiFind(GridId, Values);
	for (uint32 Value : Values)
	{
		if (FilterFunction(Value))
		{
			return Value;
		}
	}
	return NullOpt;
}

/**
 * Find only adjacent neighbours
 * @param GridLocation the location that you need the set of neighbours 
 * @return the set if neighbours ids
 */
TSet<uint32> FSpatialHash2d::GetAllAdjacentNeighboursIds(const FVector2d& GridLocation)
{
	uint32 GridID = Indexer.ToGrid(GridLocation);
	TSet<uint32> Neighbours;
	for (int Counter = -1 ; Counter <= 1; Counter +=2)
	{
		TOptional<uint32> NextRow = Indexer.GetNextRow(GridID, Counter);
		TOptional<uint32> NextColumn = Indexer.GetNextColumn(GridID, Counter);
		if (NextRow.IsSet())
		{
			Neighbours.Add(NextRow.GetValue());
		}
		if (NextColumn.IsSet())
		{
			Neighbours.Add(NextColumn.GetValue());
		}
	
	}
	return Neighbours;
}

/**
 * Find all neighbours
 * @param GridLocation the location that you need the set of neighbours 
 * @return the set if neighbours ids
 */
TSet<uint32> FSpatialHash2d::GetAllNeighboursIds(const FVector2d& GridLocation)
{
	uint32 GridID = Indexer.ToGrid(GridLocation);
	TSet<uint32> Neighbours;
	for (int RowCounter = -1 ; RowCounter <= 1; RowCounter++)
	{
		TOptional<uint32> CurrentRow = Indexer.GetNextRow(GridID, RowCounter);
		if (!CurrentRow.IsSet())
		{
			continue;
		}
		for (int ColumnCounter = -1 ; ColumnCounter <= 1; ColumnCounter++)
		{
			TOptional<uint32> CurrentColumn = Indexer.GetNextRow(CurrentRow.GetValue(), ColumnCounter);
			if (CurrentColumn.IsSet() && RowCounter != 0 && ColumnCounter != 0)
			{
				Neighbours.Add(CurrentColumn.GetValue());
			}
		}
	}
	return Neighbours;
}

uint32 FSpatialHash2d::ToGrid(const FVector2d& GridLocation) const
{
	return Indexer.ToGrid(GridLocation);
}

FVector2d FSpatialHash2d::FromGrid(uint32 GridId) const
{
	return Indexer.FromGrid(GridId);
}

uint16 FSpatialHash2d::GetCellSize() const
{
	return Indexer.CellSize;
}

