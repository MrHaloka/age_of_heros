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

FVector2d USteeringComponent::Avoidance(AMoveableUnit* MoveableObstacle, const float& TTC)
{
	UE_LOG(LogTemp, Warning, TEXT("TTC IS %f"), TTC)
	if (TTC == 0 || LastObstacleId != 0 && LastObstacleId != MoveableObstacle->GetID())
	{
		MoveableObstacle->TurnoffMovingCollisionTemporary();
		return StaticCast<AMoveableUnit*>(GetOwner())->GetVelocity2d();
	}
	double TTCWeight = (0.4-TTC)/TTC;
	LastObstacleId = MoveableObstacle->GetID();
	const FVector2d ObstacleAtCollision = MoveableObstacle->GetVelocity2d() * TTC + MoveableObstacle->GetActorLocation2d();
	const FVector2d OwnerAtCollision = StaticCast<AMoveableUnit*>(GetOwner())->GetVelocity2d() * TTC + StaticCast<AMoveableUnit*>(GetOwner())->GetActorLocation2d();
	const FVector2d AvoidanceForce = (OwnerAtCollision - ObstacleAtCollision).GetSafeNormal();
	return (StaticCast<AMoveableUnit*>(GetOwner())->GetVelocity2d().GetSafeNormal() + AvoidanceForce * TTCWeight).GetSafeNormal() * StaticCast<AMoveableUnit*>(GetOwner())->GetSpeed();
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
		double TTC = TimeToCollision(NeighbourID);
		if (FMath::IsWithin(TTC, 0.0, 0.4))
		{
			return Avoidance(StaticCast<AMoveableUnit*>(GameStatics::GetObjectManager()->GetUnits().FindChecked(NeighbourID)), TTC);
		}
	}
	LastObstacleId = 0;
	return StaticCast<AMoveableUnit*>(GetOwner())->GetVelocity2d();
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
	double Radii = Unit->GetMovingCollisionRadius() + Neighbour->GetMovingCollisionRadius();
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
