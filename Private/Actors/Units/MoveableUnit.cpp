#include "Actors/Units/MoveableUnit.h"

#include "Components/Movement/ChasingComponent.h"
#include "Components/Movement/SteeringComponent.h"
#include "DataStructures/PathLinkedList.h"
#include "GamePlay/GameStatics.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/ObjectsManager.h"
#include "Pathing/ThetaStar.h"

AMoveableUnit::AMoveableUnit() : ABaseUnit()
{
	PrimaryActorTick.bCanEverTick = true;
	CreateDefaultSubobject<USteeringComponent>(TEXT("Steering Component"));
	CreateDefaultSubobject<UChasingComponent>(TEXT("Chasing Component"));
	OnPathfindingGoalReachEvent.AddUFunction(this, "OnFinalPathfindingGoalReached");
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal"));
	SetRootComponent(SkeletalMeshComponent);
}

void AMoveableUnit::SetupPathing(FPathLinkedList* NewPath)
{
	if (Path != nullptr)
	{
		Path->DeleteList();
	}
	Path = NewPath;
	StartPathing();
	CurrentVelocity = GetComponentByClass<USteeringComponent>()->Seek(Goal);
}

void AMoveableUnit::UpdateActorRotation()
{
	FRotator ActorRotation = FVector(CurrentVelocity, 0).Rotation();
	ActorRotation.Add(0, -90, 0);
	SetActorRotation(ActorRotation);
}

void AMoveableUnit::StartPathing()
{
	bIsOnPath = true;
	Goal = GetLastPath();
	GoalVelocity = GetComponentByClass<USteeringComponent>()->Seek(Goal);
}

void AMoveableUnit::Chase(ABaseUnit* Unit, uint32 MaxDistance) const
{
	GetComponentByClass<UChasingComponent>()->BeginChase(Unit, MaxDistance);
}

void AMoveableUnit::Chase(ABaseUnit* Unit) const
{
	GetComponentByClass<UChasingComponent>()->BeginChase(Unit, GetCollisionRadius() + Unit->GetCollisionRadius());
}

void AMoveableUnit::StopChasing(EUnitState StopChasingState)
{
	GetComponentByClass<UChasingComponent>()->EndChase();
	Stop(StopChasingState);
}

void AMoveableUnit::MoveTo(const FVector2d& Location, EUnitState MovingState)
{
	FThetaStar Theta(GameStatics::GetObjectManager(),GetActorLocation2d(),Location, GetCollisionRadius());
	FPathLinkedList* NewPath = Theta.CalculateThePath();
	SetupPathing(NewPath);
	SetUnitState(MovingState);
}

float AMoveableUnit::GetSpeed() const
{
	return Speed;
}

FVector2d AMoveableUnit::GetGoalVelocity() const
{
	return GoalVelocity;
}

/**
* TODO: It's based on ThetaStar's PostProcessPrecision. Need to read it from there instead of hardcoding.
* @return The maximum tolerable collision error.
*/
float AMoveableUnit::GetAcceptableCollisionError() const
{
	return 10;
}

void AMoveableUnit::SetMovingCollisionBack()
{
	bHasMovingCollision = true;
}

void AMoveableUnit::TurnoffMovingCollisionTemporary()
{
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AMoveableUnit::SetMovingCollisionBack, 2, false);
}

void AMoveableUnit::Stop(EUnitState State)
{
	if (Path != nullptr)
	{
		Path->DeleteList();
	}
	GoalVelocity = FVector2d::Zero();
	CurrentVelocity = FVector2d::Zero();
	SetUnitState(State);
}

FVector2d AMoveableUnit::GetLastPath()
{
	FVector2d CurrentPath = Path->GetPath();
	auto OldPath = Path;
	Path = Path->GetPrevious();
	delete OldPath;
	return CurrentPath;
}

FVector2d AMoveableUnit::GetVelocity2d() const
{
	return CurrentVelocity;
}

FVector AMoveableUnit::GetVelocity() const
{
	return FVector(GetVelocity2d(), 0);
}

/**
 * Avoid calling SetActorLocation directly unless you intend to handle ObjectsManager yourself.
 * Direct calls can lead to desynchronization issues.
 * Use this method instead.
 */
void AMoveableUnit::SetActorLocation2d(const FVector2d& NewLocation)
{
	GameStatics::GetObjectManager()->UnitMoved(this, NewLocation);
	SetActorLocation(FVector(NewLocation, 0));
}

void AMoveableUnit::GoalReached()
{
	if (Path != nullptr && bIsOnPath)
	{
		StartPathing();
		return;
	}
	CurrentVelocity = FVector2d::Zero();
	GoalVelocity = FVector2d::Zero();
	OnPathfindingGoalReachEvent.Broadcast();
}

FVector2d AMoveableUnit::CalculateNewLocation(const float& DeltaSeconds)
{
	const FVector2d Movement = CurrentVelocity * DeltaSeconds;
	FVector2d NewLocation = GetActorLocation2d() + Movement;
	if (FVector2d::DistSquared(GetActorLocation2d(), Goal) <= Movement.SquaredLength())
	{
		NewLocation = Goal;
	}
	return NewLocation;
}


void AMoveableUnit::MoveTowardGoal(const float& DeltaSeconds)
{
	GoalVelocity = GetComponentByClass<USteeringComponent>()->Seek(Goal);
	CurrentVelocity = GoalVelocity;
	CurrentVelocity = GetComponentByClass<USteeringComponent>()->Avoidance();
	FVector2d NewLocation = CalculateNewLocation(DeltaSeconds);
	if (AActor* BlockingActor = CheckIfNewLocationBlocked(NewLocation))
	{
		if (BlockingActor->IsA(AMoveableUnit::StaticClass()))
		{
			DrawDebugBox(GetWorld(), GetActorLocation(), FVector(25,25,200), FColor::Red, false);
			return;
		}
	}
	SetActorLocation2d(NewLocation);
	UpdateActorRotation();
	if (GetActorLocation2d() == Goal)
	{
		GoalReached();
	}
}

void AMoveableUnit::OnFinalPathfindingGoalReached()
{
	bIsOnPath = false;
	if (GetUnitState() == Moving)
	{
		SetUnitState(Idle);
	}
}

AMoveableUnit::FOnPathfindingGoalReachEvent& AMoveableUnit::GetFinalPathfindingGoalReachEventHandler()
{
	return OnPathfindingGoalReachEvent;
}

UChasingComponent::FOnReachingTargetEvent& AMoveableUnit::GetOnReachingTargetEventHandler()
{
	return GetComponentByClass<UChasingComponent>()->OnReachingTargetEvent;
}

void AMoveableUnit::BeginPlay()
{
	Super::BeginPlay();
}

void AMoveableUnit::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (!GoalVelocity.IsZero())
	{
		MoveTowardGoal(DeltaSeconds);
	}
}

AActor* AMoveableUnit::CheckIfNewLocationBlocked(const FVector2d& NewLocation) const
{
	return GameStatics::GetObjectManager()->GetActorInLocation(NewLocation, GetCollisionRadius()-GetAcceptableCollisionError(), GetID());
}

int32 AMoveableUnit::GetMovingCollisionRadius() const
{
	if (bHasMovingCollision)
	{
		return Super::GetCollisionRadius();
	}
	return 1;
}

USkeletalMeshComponent& AMoveableUnit::GetSkeletalMeshComponent()
{
	return *SkeletalMeshComponent;
}
