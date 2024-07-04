#pragma once
#include "CoreMinimal.h"
#include "GamePlay/GameStatics.h"
#include "GamePlay/RTSGameMode.h"
#include "Helpers/GridHelper.h"

class GridHelper;

struct FHillData
{
	/**
	 * Data structure to handle hill data for creating landscape height data later.
	 * Can create and update hills, brush new hills, or add height directly to the grid.
	 */
	FHillData()
	{
		ZCellHeight = GameStatics::GetRTSGameMode()->GetZCellHeight();
		XSize = GameStatics::GetRTSGameMode()->GetGridSize() + 1;
		CellSize = GameStatics::GetRTSGameMode()->GetCellSize();
		MaxIndex = XSize * XSize;
	}
	
	/**
	 * Paints a new hill at the specified location with the given height. 
	 * Can use a brush to create a smooth hill or simply set the height.
	 * @param CurrentLocation The location of the hill.
	 * @param HeightTile The height in tiles for the hill.
	 * @param bIsBrush Determines whether to use a brush or not.
	 */
	void PaintNewHill(const FVector2d& CurrentLocation, uint16 HeightTile, bool bIsBrush)
	{
		if (bIsBrush)
		{
			BrushNewHill(CurrentLocation, HeightTile);
			return;
		}
		SetHeightValueForGrid(CurrentLocation, HeightTile);
	}
	
	/**
	 * Set height directly for all 4 corners of the grid using a weight system to calculate 
	 * the final height based on the current and new height.
	 * @param Location The location in the grid.
	 * @param HeightTile The new height in tiles to set for this grid.
	 * @param OldWeight The weight of the current height if available.
	 * @param NewWeight The weight of the new height.
	 */
	void SetHeightValueForGrid(const FVector2d& Location, uint16 HeightTile, int32 OldWeight = 1, int32 NewWeight = 1)
	{
		AddedGrids.Add(GridHelper::GetLocationOnGridCornerFromVector(Location));
		const int32 LocationIndex = GetIndexFromLocation(Location);
		AddOrUpdateHeightForGrid(LocationIndex, HeightTile, OldWeight, NewWeight);
		CreateOrUpdateHeight(LocationIndex, HeightTile, OldWeight, NewWeight);
		CreateOrUpdateHeight(LocationIndex + 1, HeightTile, OldWeight, NewWeight);
		CreateOrUpdateHeight(LocationIndex + XSize, HeightTile, OldWeight, NewWeight);
		CreateOrUpdateHeight(LocationIndex + 1 + XSize, HeightTile, OldWeight, NewWeight);
	}

	/**
	 * Brushes a new hill into the grid at the specified location with the given height in tiles.
	 * @param Location The location in the grid.
	 * @param HeightTile The height in tiles for the hill.
	 */
	void BrushNewHill(const FVector2d& Location, uint16 HeightTile)
	{
		for (int X = -HeightTile + 1 ; X <= HeightTile - 1; X++)
		{
			for (int Y = -HeightTile + 1 ; Y <= HeightTile - 1; Y++)
			{
				const int32 LocationIndex = GetIndexFromLocation(Location + FVector2d(X, Y) * CellSize);
				const FVector2d LocationOnGridCorner = GridHelper::GetLocationOnGridCornerFromVector(Location + FVector2d(X, Y) * CellSize);
				if (LocationIndex < 0 || StaticCast<uint32>(LocationIndex) > MaxIndex)
				{
					continue;
				}
				const uint32 CurrentTileHeight = (HeightTile - FMath::Max(FMath::Abs(X), FMath::Abs(Y)));
				if (Hills.Contains(LocationIndex) && Hills.FindChecked(LocationIndex) >= CurrentTileHeight)
				{
					continue;
				}
				AddedGrids.Add(LocationOnGridCorner);
				AddOrUpdateHeightForGrid(LocationIndex, CurrentTileHeight);
			}
		}
	}

	// height data for landscape
	TMap<uint32, uint32> Hills;
	// List of grids that have height data in this class
	TSet<FVector2d> AddedGrids;
	uint16 ZCellHeight;
	uint16 CellSize;
	uint16 XSize;
	uint32 MaxIndex;
protected:

	/**
	 * Calculates the height index from a given location.
	 * @param Location The location for which the index is needed.
	 * @return Index of the given location.
	 */
	int32 GetIndexFromLocation(const FVector2d& Location) const
	{
		const FVector2d Grid = GridHelper::GetGridFromVector(Location);
		return Grid.Y * XSize + Grid.X;
	}

	/**
	 * Creates or updates the height for the given grid index.
	 * @param LocationIndex The index of the grid.
	 * @param HeightTile The height of the hill in tiles.
	 */
	void AddOrUpdateHeightForGrid(uint32 LocationIndex, uint16 HeightTile, int32 OldWeight = 1, int32 NewWeight = 1)
	{
		CreateOrUpdateHeight(LocationIndex, HeightTile, OldWeight, NewWeight);
		CreateOrUpdateHeight(LocationIndex + 1, HeightTile, OldWeight, NewWeight);
		CreateOrUpdateHeight(LocationIndex + XSize, HeightTile, OldWeight, NewWeight);
		CreateOrUpdateHeight(LocationIndex + 1 + XSize, HeightTile, OldWeight, NewWeight);
	}

	/**
	 * Creates or updates the hill with the given height and weights at the specified index in the height data.
	 * @param HeightIndex The index of the hill.
	 * @param HeightTile The height of the hill in tiles.
	 * @param OldWeight The weight of the current height if available.
	 * @param NewWeight The weight of the new height.
	 */
	void CreateOrUpdateHeight(uint32 HeightIndex, uint16 HeightTile, int32 OldWeight = 1, int32 NewWeight = 1)
	{
		if (HeightIndex >= MaxIndex)
		{
			return;
		}
		if (Hills.Contains(HeightIndex))
		{
			Hills[HeightIndex] = (Hills[HeightIndex] * OldWeight + HeightTile * ZCellHeight * NewWeight) / (OldWeight + NewWeight);
		}
		else
		{
			Hills.Add(HeightIndex, HeightTile * ZCellHeight);
		}
	}
};
