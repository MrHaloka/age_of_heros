#include "ChainHandlers/CursorInputHandler/EnemyUnitHandler.h"

#include "ChainHandlers/CursorInputHandler/FriendlyMoveableHandler.h"
#include "Actors/Units/BaseUnit.h"

FEnemyUnitHandler::FEnemyUnitHandler(TSet<ABaseUnit*>& Units, TUniquePtr<FFriendlyMoveableHandler> FriendlyMoveableHandler, ETeams PlayerTeam):
Units(Units), NextHandler(MoveTemp(FriendlyMoveableHandler)), PlayerTeam(PlayerTeam)
{
}

void FEnemyUnitHandler::Execute(ABaseUnit* EnemyUnit)
{
	if (EnemyUnit->GetTeam() == PlayerTeam)
	{
		NextHandler->Execute(EnemyUnit);
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Attacking!!"))
}
