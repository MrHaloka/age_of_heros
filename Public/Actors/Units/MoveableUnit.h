#pragma once

#include "CoreMinimal.h"
#include "Actors/Units/BaseUnit.h"
#include "Components/Movement/ChasingComponent.h"
#include "Enums/UnitState.h"
#include "MoveableUnit.generated.h"

class USteeringComponent;
struct FPathLinkedList;
UCLASS()
class AOE2_API AMoveableUnit : public ABaseUnit
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USkeletalMeshComponent* SkeletalMeshComponent;
public:
	AMoveableUnit();
	void SetupPathing(FPathLinkedList* NewPath);
	void UpdateActorRotation();
	void StartPathing();
	void Chase(ABaseUnit* Unit, uint32 MaxDistance) const;
	void Chase(ABaseUnit* Unit) const;
	void StopChasing(EUnitState StopChasingState = Idle);
	virtual void MoveTo(const FVector2d& Location, EUnitState MovingState = Moving);
	float GetSpeed() const;
	FVector2d GetGoalVelocity() const;
	float GetAcceptableCollisionError() const;
	void SetMovingCollisionBack();
	void TurnoffMovingCollisionTemporary();
	void Stop(EUnitState State = Idle);
	UPROPERTY(EditDefaultsOnly, Category="movement")
	float Speed = 1;
	FVector2d GetVelocity2d() const;
	virtual FVector GetVelocity() const override;
	void SetActorLocation2d(const FVector2d& NewLocation);
	virtual int32 GetMovingCollisionRadius() const;
	USkeletalMeshComponent& GetSkeletalMeshComponent();
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
	UChasingComponent::FOnReachingTargetEvent& GetOnReachingTargetEventHandler();
};
