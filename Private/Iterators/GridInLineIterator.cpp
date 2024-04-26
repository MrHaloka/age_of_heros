#include "Iterators/GridInLineIterator.h"

#include "Helpers/GridHelper.h"


FGridInLineIterator::FGridInLineIterator(const FVector2d& StartLocation, const FVector2d& EndLocation, uint16 CellSize) :
	EndLocation(EndLocation), CellSize(CellSize)
{
	DX = abs(EndLocation.X - StartLocation.X);
	SX = StartLocation.X < EndLocation.X ? CellSize : -CellSize;
	DY = -abs(EndLocation.Y - StartLocation.Y);
	SY = StartLocation.Y < EndLocation.Y ? CellSize : -CellSize;
	Error = DX + DY;
	X0 = StartLocation.X;
	Y0 = StartLocation.Y;
}

FVector2d FGridInLineIterator::Iterate()
{
	if (abs(X0 - EndLocation.X) <= CellSize && abs(Y0 - EndLocation.Y) <= CellSize)
	{
		BEnd = true;
	}
	int32 ErrorTime2 = 2 * Error;
	if (ErrorTime2 >= DY)
	{
		if (X0 == EndLocation.X)
		{
			BEnd = true;
		}
		Error = Error + DY;
		X0 = X0 + SX;
	}
	if (ErrorTime2 <= DX)
	{
		if (Y0 == EndLocation.Y)
		{
			BEnd = true;
		}
		Error = Error + DX;
		Y0 = Y0 + SY;
	}
	return FVector2d(X0, Y0);
}

bool FGridInLineIterator::IsFinish()
{
	return BEnd;
}
