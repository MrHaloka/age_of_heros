#include "Commands/BaseUnitCommand.h"

FBaseUnitCommand::FBaseUnitCommand(TSet<ABaseUnit*> Units): Units(Units){}

FBaseUnitCommand::~FBaseUnitCommand()
{
}

void FBaseUnitCommand::RunCommand()
{
}

