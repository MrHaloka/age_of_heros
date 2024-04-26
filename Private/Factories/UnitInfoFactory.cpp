#include "Factories/UnitInfoFactory.h"

FUnitInfoFactory::FUnitInfoFactory(TSubclassOf<ABaseUnit> UnitInfo, FVector2d UnitSize) :
UnitInfo(UnitInfo), UnitSize(UnitSize)
{
}

TSubclassOf<ABaseUnit> FUnitInfoFactory::GetUnitInfo() const
{
	return UnitInfo;
}

FVector2d FUnitInfoFactory::GetUnitSize() const
{
	return UnitSize;
}
