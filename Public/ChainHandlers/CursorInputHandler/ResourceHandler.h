#pragma once
#include "CoreMinimal.h"

class ABaseUnit;
class FEnemyUnitHandler;

class AOE2_API FResourceHandler
{
public:
	FResourceHandler(TSet<ABaseUnit*>& Units, TUniquePtr<FEnemyUnitHandler> EnemyUnitHandler);
	void Execute(AActor* Resource);
protected:
	TUniquePtr<FEnemyUnitHandler> NextHandler;
	TSet<ABaseUnit*>& Units;
};
