#include "ChainHandlers/CursorInputHandler/FriendlyMoveableHandler.h"

#include "ChainHandlers/CursorInputHandler/ConstructionSiteHandler.h"
#include "GamePlay/PlayerSpectatorPawn.h"
#include "Actors/Units/BaseUnit.h"
#include "Actors/Units/MoveableUnit.h"

FFriendlyMoveableHandler::FFriendlyMoveableHandler(TSet<ABaseUnit*>& Units, APlayerSpectatorPawn* Player, TUniquePtr<FConstructionSiteHandler> ConstructionSiteHandler):
Units(Units), NextHandler(MoveTemp(ConstructionSiteHandler)), Player(Player)
{
}

void FFriendlyMoveableHandler::Execute(ABaseUnit* Unit)
{
	if (!Unit->IsA(AMoveableUnit::StaticClass()))
	{
		NextHandler->Execute(Unit);
		return;
	}
	Units.Add(Unit);
	Unit->ShowUnitPanel(Player->GetRTSHUD());
}
