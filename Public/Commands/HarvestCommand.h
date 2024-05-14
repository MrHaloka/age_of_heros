#pragma once
#include "BaseUnitCommand.h"

class ABaseResources;

class AOE2_API HarvestCommand : public FBaseUnitCommand
{
public:
	explicit HarvestCommand(TSet<ABaseUnit*> Units, ABaseResources* TargetResource);
	virtual void RunCommand() override;
protected:
	ABaseResources* TargetResource;
	
};
