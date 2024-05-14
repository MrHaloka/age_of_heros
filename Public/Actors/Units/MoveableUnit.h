#pragma once

#include "CoreMinimal.h"
#include "Actors/Units/BaseUnit.h"
#include "Enums/UnitState.h"
#include "MoveableUnit.generated.h"

class USteeringComponent;
struct FPathLinkedList;
UCLASS()
class AOE2_API AMoveableUnit : public ABaseUnit
{
	GENERATED_BODY()

public:
	AMoveableUnit();
	void SetIsMoving(bool BMoving);
	void SetupPathing(FPathLinkedList* NewPath);
	void UpdateActorRotation();
	void StartPathing();
	virtual void MoveTo(const FVector2d& Location, EUnitState MovingState = Moving);
	float GetSpeed() const;
	FVector2d GetGoalVelocity() const;
	float GetAcceptableCollisionError() const;
	void SetMovingCollisionBack();
	void TurnoffMovingCollisionTemporary();
	UPROPERTY(EditDefaultsOnly)
	float Speed = 1;
	FVector2d GetVelocity2d() const;
	virtual FVector GetVelocity() const override;
	void SetActorLocation2d(const FVector2d& NewLocation);
	virtual int32 GetMovingCollisionRadius() const;
protected:
	void GoalReached();
	FVector2d CalculateNewLocation(const float& DeltaSeconds);
	void MoveTowardGoal(const float& DeltaSeconds);
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	AActor* CheckIfNewLocationBlocked(const FVector2d& NewLocation) const;
	bool bIsMoving = false;
	bool bIsOnPath = false;
	bool bHasMovingCollision = true;
	FPathLinkedList*  Path = nullptr;
	FVector2d GetLastPath();
	FVector2d GoalVelocity = FVector2d::Zero();
	FVector2d CurrentVelocity = FVector2d::Zero();
	FVector2d Goal;
protected:
	DECLARE_EVENT(AMoveableUnit, FOnPathfindingGoalReachEvent);
	FOnPathfindingGoalReachEvent OnPathfindingGoalReachEvent;
	UFUNCTION()
	virtual void OnFinalPathfindingGoalReached();
public:
	FOnPathfindingGoalReachEvent& GetFinalPathfindingGoalReachEventHandler();
};
