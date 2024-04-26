#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ResourceHarvestingComponent.generated.h"

class AVillager;
class FHarvestingManager;
class ABaseResources;
struct FResourcesInfo;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
/**
 * Need a movable type with inventory functionality.
 * Currently, only the villager actor is capable of gathering resources, but this may change in the future.
 */
class AOE2_API UResourceHarvestingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UResourceHarvestingComponent();
	void Initialize(ABaseResources* Resources);
	void StopHarvesting();
	void GoBackToResource();
	
protected:
	virtual void BeginPlay() override;
	FHarvestingManager* HarvestingManager;
	uint32 ResourceId;
	const FResourcesInfo* ResourceInfo;
	FVector2d SlotLocation;
	UPROPERTY()
	ABaseResources* TargetResource;
	ABaseResources* GetNewResourceTarget();
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
