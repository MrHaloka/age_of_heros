#include "Components/SteeringComponent.h"

#include "Actors/Units/MoveableUnit.h"
#include "GamePlay/GameStatics.h"
#include "Managers/ObjectsManager.h"

USteeringComponent::USteeringComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

FVector2d USteeringComponent::Seek(FVector2d Goal)
{
	const FVector2d GoalDirection = (Goal - StaticCast<AMoveableUnit*>(GetOwner())->GetActorLocation2d()).GetSafeNormal();
	return GoalDirection * StaticCast<AMoveableUnit*>(GetOwner())->GetSpeed();
}

void USteeringComponent::AddActiveIgnoreId(uint32 Id)
{
	ActiveIgnoreId = Id;
	GetWorld()->GetTimerManager().SetTimer(IgnoreTimer, this, &USteeringComponent::RemoveIgnoreId, 1.5, false);
}

FVector2d USteeringComponent::Avoidance(AMoveableUnit* MoveableObstacle, const float& TTC)
{
	UE_LOG(LogTemp, Warning, TEXT("TTC IS %f"), TTC);
	const float TTCWeight = FMath::Square(TTC) / 16;
	const FVector2d OwnerDirection = StaticCast<AMoveableUnit*>(GetOwner())->GetVelocity2d().GetSafeNormal();
	const FVector2d ObstacleDirection = MoveableObstacle->GetVelocity2d().GetSafeNormal();
	const float DotProduct = FVector2d::DotProduct(OwnerDirection, ObstacleDirection);
	if (!ObstacleDirection.IsZero() && DotProduct > -0.2) 
	{
		MoveableObstacle->GetComponentByClass<USteeringComponent>()->AddActiveIgnoreId(StaticCast<AMoveableUnit*>(GetOwner())->GetID());
		return StaticCast<AMoveableUnit*>(GetOwner())->GetVelocity2d() * TTCWeight ;
	}
	const FVector2d LeftSide = (OwnerDirection + FVector2d(OwnerDirection.Y, -OwnerDirection.X)).GetSafeNormal();
	const FVector2d RightSide = (OwnerDirection + FVector2d(-OwnerDirection.Y, OwnerDirection.X)).GetSafeNormal();
	const FVector2d DirectionTowardObstacle = MoveableObstacle->GetActorLocation2d() - StaticCast<AMoveableUnit*>(GetOwner())->GetActorLocation2d();
	const FVector2d RelativeVelocity = MoveableObstacle->GetVelocity2d() - StaticCast<AMoveableUnit*>(GetOwner())->GetVelocity2d();
	const float DirectionDotVelocity = DirectionTowardObstacle.Dot(RelativeVelocity);
	if (DirectionDotVelocity < 0)
	{
		return (RightSide + OwnerDirection * TTCWeight).GetSafeNormal() * StaticCast<AMoveableUnit*>(GetOwner())->GetSpeed();
	}
	return (LeftSide + OwnerDirection * TTCWeight).GetSafeNormal() * StaticCast<AMoveableUnit*>(GetOwner())->GetSpeed();
}

FVector2d USteeringComponent::Avoidance()
{
	check(GetOwner()->IsA(AMoveableUnit::StaticClass()))
	TSet<uint32> Neighbours = GameStatics::GetObjectManager()->GetNeighboursMoveable(
		StaticCast<AMoveableUnit*>(GetOwner())->GetActorLocation2d(),
		StaticCast<AMoveableUnit*>(GetOwner())->GetID(),
		200
		);
	for (uint32 NeighbourID : Neighbours)
	{
		if (NeighbourID == ActiveIgnoreId)
		{
			continue;
		}
		double TTC = TimeToCollision(NeighbourID) * 10;
		if (FMath::IsWithin(TTC, 0, 4))
		{
			return Avoidance(StaticCast<AMoveableUnit*>(GameStatics::GetObjectManager()->GetUnits().FindChecked(NeighbourID)), TTC); 
		}
	}
	return StaticCast<AMoveableUnit*>(GetOwner())->GetVelocity2d();
}

void USteeringComponent::RemoveIgnoreId()
{
	ActiveIgnoreId = 0;
}

/**
 * We use the following equation to calculate the time to collision:
 * This equation is based on (LocationUnit+VelocityUnit*τ)-(LocationNeighbour+VelocityNeighbour*τ) = Radius Unit + Neighbour
 * (V.V) τ^2 + ⋅ (2* w.v ) τ + ⋅w.w −  (r1 + r2)^2 = 0
 * @param W = unit's location - Neighbour's location 
 * @param V = unit's Velocity  - Neighbour's Velocity 
 * @param A = dot(v, v)
 * @param B = 2*dot(v, w)
 * @param C = dot(w, w) - (r1 + r2)^2
 * @return The time left to collision
 */
double USteeringComponent::TimeToCollision(uint32 NeighbourId)
{
	const AMoveableUnit* Unit = StaticCast<AMoveableUnit*>(GetOwner());
	const AMoveableUnit* Neighbour = StaticCast<AMoveableUnit*>(GameStatics::GetObjectManager()->GetUnits().FindChecked(NeighbourId));
	double Radii = Unit->GetCollisionRadius() + Neighbour->GetCollisionRadius();
	FVector2d W = FVector2d(Neighbour->GetActorLocation() - Unit->GetActorLocation());
	double C = W.Dot(W) - (Radii * Radii);
	// already colliding 
	if (C < 0)
	{
		return 0;
	}
	FVector2d V = FVector2d(Unit->GetVelocity() - Neighbour->GetVelocity());
	double A = V.Dot(V);
	double B = V.Dot(W);
	double Discriminant = B * B - A * C;
	if (Discriminant <= 0)
	{
		return -1;
	}
	double Tau = (B - FMath::Sqrt(Discriminant)) / A;
	if (Tau < 0)
	{
		return -1;
	}
	return Tau;
}
