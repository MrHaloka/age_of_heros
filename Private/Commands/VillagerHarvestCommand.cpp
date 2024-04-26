#include "Commands/VillagerHarvestCommand.h"

#include "Actors/Units/Villager.h"

VillagerHarvestCommand::VillagerHarvestCommand(TSet<ABaseUnit*> Units, ABaseResources* TargetResource) :
FBaseUnitCommand(Units), TargetResource(TargetResource)
{
}

void VillagerHarvestCommand::RunCommand()
{
	FBaseUnitCommand::RunCommand();
	for (ABaseUnit* Unit : Units)
	{
		if (AVillager* Villager = Cast<AVillager>(Unit))
		{
			Villager->GatherResource(TargetResource);
		}
	}
}
