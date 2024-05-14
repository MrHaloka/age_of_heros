#include "ChainHandlers/CursorInputHandler/ConstructionSiteHandler.h"

#include "ChainHandlers/CursorInputHandler/BuildingHandler.h"
#include "Commands/ConstructCommand.h"
#include "Actors/Units/Buildings/BaseBuilding.h"
#include "Actors/Units/BuildingConstruction.h"

FConstructionSiteHandler::FConstructionSiteHandler(TSet<ABaseUnit*>& Units, TUniquePtr<FBuildingHandler> BuildingHandler):
Units(Units), NextHandler(MoveTemp(BuildingHandler))
{
}

void FConstructionSiteHandler::Execute(ABaseUnit* TargetBuilding)
{
	if (!TargetBuilding->IsA(ABuildingConstruction::StaticClass()))
	{
		NextHandler->Execute(reinterpret_cast<ABaseBuilding*>(TargetBuilding));
		return;
	}
	ConstructCommand ConstructCommand(Units, reinterpret_cast<ABuildingConstruction*>(TargetBuilding));
	ConstructCommand.RunCommand();
}
