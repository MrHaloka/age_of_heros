#pragma once
#include "CoreMinimal.h"

enum class ETeams;
class FFriendlyMoveableHandler;
class ABaseUnit;

class AOE2_API FEnemyUnitHandler
{
public:
	FEnemyUnitHandler(TSet<ABaseUnit*>& Units, TUniquePtr<FFriendlyMoveableHandler> FriendlyMoveableHandler, ETeams PlayerTeam);
	void Execute(ABaseUnit* EnemyUnit);
protected:
	TSet<ABaseUnit*>& Units;
	const ETeams PlayerTeam;
	TUniquePtr<FFriendlyMoveableHandler> NextHandler;
};
