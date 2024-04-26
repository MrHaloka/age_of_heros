#pragma once
#include "CoreMinimal.h"

class ABaseBuilding;
class APlayerSpectatorPawn;
class ABaseUnit;

class AOE2_API FBuildingHandler
{
public:
	FBuildingHandler(TSet<ABaseUnit*>& Units, APlayerSpectatorPawn& Player);
	void Execute(ABaseBuilding* Building);
protected:
	TSet<ABaseUnit*>& Units;
	APlayerSpectatorPawn& Player;	
};
