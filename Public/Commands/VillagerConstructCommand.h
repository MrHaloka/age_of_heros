#pragma once
#include "BaseUnitCommand.h"

class ABuildingConstruction;
class ABaseBuilding;

class AOE2_API FVillagerConstructCommand: public FBaseUnitCommand
{
public:
	explicit FVillagerConstructCommand(TSet<ABaseUnit*> Units, ABuildingConstruction* TargetBuilding);
	virtual void RunCommand() override;
protected:
	ABuildingConstruction* TargetBuilding;
};
