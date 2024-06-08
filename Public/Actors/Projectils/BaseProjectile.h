#pragma once

#include "CoreMinimal.h"
#include "Enums/Teams.h"
#include "GameFramework/Actor.h"
#include "BaseProjectile.generated.h"

UCLASS()
class AOE2_API ABaseProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseProjectile();
	virtual void SetDamage(const float& ProjectileDamage);
	virtual FTransform InitializeProjectile(const FVector2d& SpawnLocation, const FVector& ProjectileTargetLocation,  const uint32& ZOffset);
	float GetSpeed() const;
	float GetCollisionRadius() const;
protected:
	virtual void BeginPlay() override;
	virtual void CheckForHit(const FVector& NewLocation);
	ETeams GetTeam() const;
protected:
	uint32 Damage;
	FVector Velocity;
	static constexpr float CustomGravity = 8000;
public:	
	virtual void Tick(float DeltaTime) override;
	
};
