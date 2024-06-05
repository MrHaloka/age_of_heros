#pragma once

#include "CoreMinimal.h"
#include "BaseCombatComponent.h"
#include "MeleeAttackComponent.generated.h"


class ABaseUnit;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AOE2_API UMeleeAttackComponent : public UBaseCombatComponent
{
	GENERATED_BODY()
};
