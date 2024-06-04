#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enums/UnitState.h"
#include "ChasingComponent.generated.h"

enum EUnitState : int;
class AMoveableUnit;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AOE2_API UChasingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UChasingComponent();
	void BeginChase(ABaseUnit* NewTarget, uint32 MaxDistance);
	void BeginChase(AMoveableUnit* NewTarget, uint32 MaxDistance);
	UFUNCTION()
	void EndChase();
protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	void MoveTowardsTarget();
	UFUNCTION()
	void OnTargetDestroyed(AActor* Actor, EEndPlayReason::Type EndPlayReason);
	void HasReachedTarget();
protected:
	bool bIsReached = false;
	FTimerHandle ChasingTimer;
	UPROPERTY()
	AMoveableUnit* Target;
	//Maximum acceptable distance that will trigger the "FOnReachingTargetEvent"
	uint32 MaxDistanceSqr;
public:
	DECLARE_EVENT(AMoveableUnit, FOnReachingTargetEvent);
	FOnReachingTargetEvent OnReachingTargetEvent;
	UFUNCTION()
	void OwnerPrepareToStateChangeListener(TEnumAsByte<EUnitState> NewState, TEnumAsByte<EUnitState> OldState);
};
