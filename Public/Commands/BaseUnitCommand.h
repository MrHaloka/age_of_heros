#pragma once
#include "CoreMinimal.h"

class ABaseUnit;

class AOE2_API FBaseUnitCommand
{
public:
	FBaseUnitCommand(TSet<ABaseUnit*> Units);
	virtual ~FBaseUnitCommand();
	virtual void RunCommand();
protected:
	TSet<ABaseUnit*> Units;
};
