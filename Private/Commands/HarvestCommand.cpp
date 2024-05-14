#include "Commands/HarvestCommand.h"

#include "Actors/Units/Villager.h"
#include "Components/ResourceHarvestingComponent.h"

HarvestCommand::HarvestCommand(TSet<ABaseUnit*> Units, ABaseResources* TargetResource) :
FBaseUnitCommand(Units), TargetResource(TargetResource)
{
}

void HarvestCommand::RunCommand()
{
	FBaseUnitCommand::RunCommand();
	for (ABaseUnit* Unit : Units)
	{
		if (UResourceHarvestingComponent* HarvestingComponent = Unit->GetComponentByClass<UResourceHarvestingComponent>())
		{
			HarvestingComponent->Initialize(TargetResource);
		}
	}
}
