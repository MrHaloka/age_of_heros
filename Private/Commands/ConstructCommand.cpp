#include "Commands/ConstructCommand.h"

#include "Actors/Units/BuildingConstruction.h"
#include "Components/BuilderComponent.h"

ConstructCommand::ConstructCommand(TSet<ABaseUnit*> Units, ABuildingConstruction* TargetBuilding) :
FBaseUnitCommand(Units), TargetBuilding(TargetBuilding)
{
}

void ConstructCommand::RunCommand()
{
	FBaseUnitCommand::RunCommand();
	for (ABaseUnit* Unit : Units)
    {
    	if (UBuilderComponent* BuilderComponent = Unit->GetComponentByClass<UBuilderComponent>())
    	{
    			BuilderComponent->Initialize(TargetBuilding);
    	}
    }
}
