#pragma once
#include "CoreMinimal.h"

class FGridInLineIterator;
class FObjectsManager;

class AOE2_API FLineOfSightComponent
{
public:
	FLineOfSightComponent(FObjectsManager* ObjectsManager);
	TArray<FVector2d> GetBlockingObjects(FGridInLineIterator GridIterator) const;
	bool HasLineOfSight(FGridInLineIterator GridIterator) const;

protected:
	FObjectsManager* ObjectsManager;
};
