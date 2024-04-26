﻿#pragma once
#include "BaseUnitCommand.h"

class ABaseResources;

class AOE2_API VillagerHarvestCommand : public FBaseUnitCommand
{
public:
	explicit VillagerHarvestCommand(TSet<ABaseUnit*> Units, ABaseResources* TargetResource);
	virtual void RunCommand() override;
protected:
	ABaseResources* TargetResource;
	
};
