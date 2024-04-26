#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SteeringComponent.generated.h"

class AMoveableUnit;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AOE2_API USteeringComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	USteeringComponent();
	FVector2d Seek(FVector2d Goal);
	void Flee();
	void AddActiveIgnoreId(uint32 Id);
	FVector2d Avoidance(AMoveableUnit* MoveableUnit, const float& TTC);
	FVector2d Avoidance();
protected:
	void RemoveIgnoreId();
	double TimeToCollision(uint32 NeighbourId);
	uint32 ActiveIgnoreId = 0;
	FTimerHandle IgnoreTimer;
};
