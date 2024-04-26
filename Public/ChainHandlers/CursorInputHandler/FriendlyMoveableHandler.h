#pragma once
#include "CoreMinimal.h"

class APlayerSpectatorPawn;
class FConstructionSiteHandler;
class ABaseUnit;

class AOE2_API FFriendlyMoveableHandler
{
public:
	FFriendlyMoveableHandler(TSet<ABaseUnit*>& Units, APlayerSpectatorPawn* Player, TUniquePtr<FConstructionSiteHandler> ConstructionSiteHandler);
	void Execute(ABaseUnit* Unit);
protected:
	TUniquePtr<FConstructionSiteHandler> NextHandler;
	APlayerSpectatorPawn* Player;
	TSet<ABaseUnit*>& Units;
};
