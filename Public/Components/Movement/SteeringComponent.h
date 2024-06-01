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
	FVector2d Avoidance();
	FVector2d Avoidance(AMoveableUnit* MoveableUnit, const float& TTC);
	void Flee();
protected:
	void RemoveIgnoreId();
	double TimeToCollision(uint32 NeighbourId);
	uint32 LastObstacleId = 0;
};
