#pragma once

#include "CoreMinimal.h"
#include "Actors/Units/BaseUnit.h"
#include "Components/ActorComponent.h"
#include "SightComponent.generated.h"

class AMoveableUnit;
class ABaseUnit;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AOE2_API USightComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USightComponent();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	UPROPERTY(EditDefaultsOnly, Category="Perception|Raduis")
	int32 PerceptionRadius;
public:	
	FVector2d GetPerceptionCenter();
	uint32 GetPerceptionRadius();
	uint32 GetPerceptionRadiusSquare();
	ABaseUnit* GetFirstEnemyInPerception();
	UFUNCTION()
	void Sensed(const ABaseUnit* Unit);
public:
	DECLARE_EVENT_OneParam(USightComponent, FOnSenseUnitEvent, const ABaseUnit*);
	FOnSenseUnitEvent OnSenseUnitEvent;
};
