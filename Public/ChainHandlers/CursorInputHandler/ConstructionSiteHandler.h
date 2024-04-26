#pragma once
#include "CoreMinimal.h"

class APlayerSpectatorPawn;
class ABaseBuilding;
class FBuildingHandler;
class ABaseUnit;

class AOE2_API FConstructionSiteHandler
{
public:
	FConstructionSiteHandler(TSet<ABaseUnit*>& Units, TUniquePtr<FBuildingHandler> BuildingHandler);
	void Execute(ABaseUnit* Building);
protected:
	TSet<ABaseUnit*>& Units;
	TUniquePtr<FBuildingHandler> NextHandler;

};
