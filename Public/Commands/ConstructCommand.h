#pragma once
#include "BaseUnitCommand.h"

class ABuildingConstruction;
class ABaseBuilding;

class AOE2_API ConstructCommand: public FBaseUnitCommand
{
public:
	explicit ConstructCommand(TSet<ABaseUnit*> Units, ABuildingConstruction* TargetBuilding);
	virtual void RunCommand() override;
protected:
	ABuildingConstruction* TargetBuilding;
};
