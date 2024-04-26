#include "ChainHandlers/CursorInputHandler/BuildingHandler.h"

#include "GamePlay/PlayerSpectatorPawn.h"
#include "Actors/Units/Buildings/BaseBuilding.h"

FBuildingHandler::FBuildingHandler(TSet<ABaseUnit*>& Units, APlayerSpectatorPawn& Player):
Units(Units), Player(Player)
{
}

void FBuildingHandler::Execute(ABaseBuilding* Building)
{
	Units.Empty();
	Building->ShowUnitPanel(Player.GetRTSHUD());
}
