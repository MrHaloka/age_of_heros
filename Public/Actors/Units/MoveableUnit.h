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
	UPROPERTY(EditDefaultsOnly)
	float Speed = 1;
	FVector2d GetVelocity2d() const;
	virtual FVector GetVelocity() const override;

protected:
	void GoalReached();
	void MoveTowardGoal(const float& DeltaSeconds);
	virtual void FinalPathfindingGoalReached();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	bool CheckIfNewLocationBlocked(const FVector2d& NewLocation) const;
	bool BIsMoving = false;
	bool BIsOnPath = false;
	FPathLinkedList*  Path = nullptr;
	FVector2d GetLastPath();
	FVector2d GoalVelocity = FVector2d::Zero();
	FVector2d CurrentVelocity = FVector2d::Zero();
	FVector2d Goal;
};
