#include "ChainHandlers/CursorInputHandler/ConstructionSiteHandler.h"

#include "ChainHandlers/CursorInputHandler/BuildingHandler.h"
#include "Commands/VillagerConstructCommand.h"
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
	FVillagerConstructCommand VillagerConstructCommand(Units, reinterpret_cast<ABuildingConstruction*>(TargetBuilding));
	VillagerConstructCommand.RunCommand();
}
