#pragma once
#include "CoreMinimal.h"
#include "BaseUnitCommand.h"

class AOE2_API FMoveCommand : public FBaseUnitCommand
{
public:
	explicit FMoveCommand(TSet<ABaseUnit*> Units, const FVector2d& Location);
	virtual void RunCommand() override;
protected:
	FVector2d Location;
};
