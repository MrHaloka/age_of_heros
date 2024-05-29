#pragma once

#include "CoreMinimal.h"
#include "Actors/Units/BaseUnit.h"

class FObjectsManager;
class AMoveableUnit;
class ABaseUnit;
class FSpatialHash2d;

class AOE2_API FPerceptionManager 
{
public:
	FPerceptionManager(FObjectsManager* InObjectsManager, uint8 GridSize, uint16 NewCellSize);
	void UnitAddedToGridListener(const ABaseUnit* AddedUnit, bool bIsHologram = false) const;
	void UnitMovedListener(AMoveableUnit* Moved, const FVector2D& NewLocation);
	void RegisterUnitSightPerception(const ABaseUnit* Unit);
	void RemoveUnitSightPerception(const ABaseUnit* Unit);
protected:
	TUniquePtr<FSpatialHash2d> PerceptionGrid;
	void NotifyObserver(const ABaseUnit* Observer, const uint32& SensedUnitId);
	void CheckSensedUnits(const ABaseUnit* Observer, const TSet<uint32>& SensedUnits);
	void CheckSensedUnits(const ABaseUnit* Observer, const TArray<uint32>& SensedUnits);
	void UpdatePerceptionGrid(AMoveableUnit* Moved, const FVector2D& NewLocation);
	uint16 CellSize;
	FObjectsManager* ObjectsManager;
};
