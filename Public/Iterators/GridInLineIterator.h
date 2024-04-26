#pragma once

class AOE2_API FGridInLineIterator
{
public:
	FGridInLineIterator(const FVector2d& StartLocation, const FVector2d& EndLocation, uint16 CellSize);
	FVector2d Iterate();
	bool IsFinish();
protected:
	const FVector2d& EndLocation;
	int32 DX;
	int32 SX; 
	int32 DY;
	int32 SY;
	int32 Error;
	int32 X0;
	int32 Y0;
	uint16 CellSize;
	bool BEnd = false;
};
