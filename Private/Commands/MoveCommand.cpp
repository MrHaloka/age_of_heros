#include "Commands/MoveCommand.h"

#include "Actors/Units/MoveableUnit.h"

FMoveCommand::FMoveCommand(TSet<ABaseUnit*> Units, const FVector2d& Location) : FBaseUnitCommand(Units) ,Location(Location)
{
	
}

void FMoveCommand::RunCommand()
{
	FBaseUnitCommand::RunCommand();
	for (ABaseUnit* Unit : Units)
	{
		if (AMoveableUnit* MoveableUnit = Cast<AMoveableUnit>(Unit))
		{
			MoveableUnit->MoveTo(Location);
		}
	}
}
