#include "Actors/Projectils/BaseProjectile.h"

#include "Actors/Units/BaseUnit.h"
#include "DataStructures/ProjectileInfo.h"
#include "Engine/DamageEvents.h"
#include "Enums/Teams.h"
#include "GamePlay/GameStatics.h"
#include "Managers/ObjectsManager.h"

ABaseProjectile::ABaseProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
}
void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	check(GetOwner()->IsA(ABaseUnit::StaticClass()))
}

/**
 * Checks if the new location collides with any actor. If it does, the actor takes damage.
 * TODO::Continuous collision detection logic may need to be implemented in the future.
 * @param NewLocation The location where the projectile will be in this tick.
 */
void ABaseProjectile::CheckForHit(const FVector& NewLocation)
{
	if (AActor* Actor = GameStatics::GetObjectManager()->GetActorInLocation(FVector2d(NewLocation), GetTeam(), GetCollisionRadius()))
	{
		const FDamageEvent DamageEvent;
		Actor->TakeDamage(Damage, DamageEvent, GetWorld()->GetFirstPlayerController(), GetOwner());
		Destroy();
	}
}

void ABaseProjectile::SetDamage(const float& ProjectileDamage)
{
	Damage = ProjectileDamage;
}

/**
 * Initializes the projectile's spawn location and rotation, and sets the velocity based on the target location.
 * @param SpawnLocation The location where the projectile spawns.
 * @param ProjectileTargetLocation The location of the target it's aiming at.
 * @param ZOffset The vertical offset of the projectile.
 * @return The calculated transform that the projectile should spawn with.
 */
FTransform ABaseProjectile::InitializeProjectile(const FVector2d& SpawnLocation, const FVector& ProjectileTargetLocation, const uint32& ZOffset)
{
	const float DeltaZ = ProjectileTargetLocation.Z - ZOffset;
	const float Length2d = FVector2d::Distance(SpawnLocation, FVector2d(ProjectileTargetLocation));
	const float TotalTimeToReach2d = Length2d / GetSpeed();
	const float ZVelocity = (DeltaZ / TotalTimeToReach2d) + ((CustomGravity * TotalTimeToReach2d) /2);
	const FVector ForwardDirection = (ProjectileTargetLocation - FVector(SpawnLocation, 0)).GetSafeNormal();
	Velocity = FVector(FVector2d(ForwardDirection * GetSpeed()), ZVelocity);
	FRotator ForwardRotation = ForwardDirection.Rotation();
	FTransform ForwardTransform;
	ForwardTransform.SetLocation(FVector(SpawnLocation, ZOffset));
	ForwardTransform.SetRotation(FQuat(ForwardRotation));
	return ForwardTransform;
}

/**
 * Updates the location each tick and checks for any hit events.
 */
void ABaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Velocity.Z -= CustomGravity * DeltaTime;
	const FVector NewLocation = GetActorLocation() + Velocity * DeltaTime;
	CheckForHit(NewLocation);
	SetActorLocation(NewLocation);
	SetActorRotation(Velocity.Rotation());
	if (GetActorLocation().Z < 0)
	{
		Destroy();		
	}
}

ETeams ABaseProjectile::GetTeam() const
{
	return StaticCast<ABaseUnit*>(GetOwner())->GetTeam();
}

float ABaseProjectile::GetSpeed() const
{
	return GameStatics::GetProjectileInfoByClassName(GetClass()->GetFName())->Speed;
}

float ABaseProjectile::GetCollisionRadius() const
{
	return GameStatics::GetProjectileInfoByClassName(GetClass()->GetFName())->CollisionRadius;
}
