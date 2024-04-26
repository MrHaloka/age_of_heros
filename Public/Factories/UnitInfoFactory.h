#pragma once

class ABaseUnit;

class AOE2_API FUnitInfoFactory
{
public:
	FUnitInfoFactory(TSubclassOf<ABaseUnit> UnitInfo, FVector2d UnitSize = FVector2d::Zero());
	TSubclassOf<ABaseUnit> GetUnitInfo() const;
	FVector2d GetUnitSize() const;
protected:
	FVector2d UnitSize;
	TSubclassOf<ABaseUnit> UnitInfo;
};
