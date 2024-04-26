#include "Commands/VillagerConstructCommand.h"

#include "Actors/Units/Villager.h"
#include "Actors/Units/BuildingConstruction.h"

FVillagerConstructCommand::FVillagerConstructCommand(TSet<ABaseUnit*> Units, ABuildingConstruction* TargetBuilding) :
FBaseUnitCommand(Units), TargetBuilding(TargetBuilding)
{
}

void FVillagerConstructCommand::RunCommand()
{
	FBaseUnitCommand::RunCommand();
	for (ABaseUnit* Unit : Units)
    {
    	if (AVillager* Villager = Cast<AVillager>(Unit))
    	{
    			Villager->GoToConstructionSite(TargetBuilding);
    	}
    }
}
