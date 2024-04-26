#pragma once
#include "CoreMinimal.h"

class FObjectsManager;
class ABaseUnit;
class FResourceHandler;

class AOE2_API FEmptyLocationHandler
{
public:
	FEmptyLocationHandler(TSet<ABaseUnit*>& Units, TUniquePtr<FResourceHandler> ResourceHandler);
	void Execute(AActor* NewSelectedUnit,const FVector2d& Location);
protected:
	TSet<ABaseUnit*>& Units;
	TUniquePtr<FResourceHandler> NextHandler;
};
