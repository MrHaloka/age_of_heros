#pragma once
#include "DataStructures/GridIndexer2d.h"

class AOE2_API FSpatialHash2d
{
private:
	TMultiMap<uint32, uint32> Hash;
	FCriticalSection CriticalSection;
	FGridIndexer2d Indexer;

public:
	FSpatialHash2d(const uint8 GridSize, const uint16 CellSize);
	void InsertPoint(const uint32 PointID, const FVector2d& Location);
	void InsertPoints(uint32 PointID, const FVector2d& Location, const FVector2d& UnitSize);
	void InsertPointUnsafe(uint32 PointID, const FVector2d& Location);
	bool RemovePoint(uint32 PointID, const FVector2d& Location);
	bool RemovePointUnsafe(uint32 PointID, const FVector2d& Location);
	void RemovePointsUnsafe(const uint32 PointID, const FVector2d& Location, const FVector2d& UnitSize);
	void UpdatePoint(uint32 PointID, const FVector2d& OldLocation, const FVector2d& CurrentLocation);
	void UpdatePointUnsafe(uint32 PointID, const FVector2d& OldLocation, const FVector2d& CurrentLocation);
	TOptional<uint32> FindNearestInRadius(
		const FVector2d& QueryPoint,
		float Radius,
		TFunctionRef<float(uint32)> DistanceSqFunc,
		TFunctionRef<bool(uint32)> IgnoreFunc = [](uint32) {return false;}) const;
	bool IsAnyInRadius(
		const FVector2d& QueryPoint,
		float Radius,
		TFunctionRef<float(uint32)> DistanceSqFunc,
		TFunctionRef<bool(uint32)> IgnoreFunc = [](uint32) {return false;}) const;
	TSet<uint32> FindAllInRadius(
		const FVector2d& QueryPoint,
		float Radius,
		TFunctionRef<float(const uint32&)> DistanceSqFunc,
		TFunctionRef<bool(const uint32&)> IgnoreFunc = [](uint32) {return false;}) const;
	TOptional<uint32> GetPointInGrid(const FVector2d& GridLocation, TFunctionRef<bool(uint32)> FilterFunction);
	TOptional<uint32> GetPointInGrid(uint32 GridId, TFunctionRef<bool(uint32)> FilterFunction);
	TSet<uint32> GetAllAdjacentNeighboursIds(const FVector2d& GridLocation);
	TSet<uint32> GetAllNeighboursIds(const FVector2d& GridLocation);
	uint32 ToGrid(const FVector2d& GridLocation) const;
	FVector2d FromGrid(uint32 Id) const;
	uint16 GetCellSize() const;
};
