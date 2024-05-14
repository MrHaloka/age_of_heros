#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BuilderComponent.generated.h"

class ABuildingConstruction;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AOE2_API UBuilderComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBuilderComponent();
	UFUNCTION()
	void OwnerPrepareToStateChangeListener(TEnumAsByte<EUnitState> NewState, TEnumAsByte<EUnitState> OldState);
protected:
	virtual void BeginPlay() override;
	FVector2d SlotLocation;
	UPROPERTY()
	ABuildingConstruction* BuildingTarget;
public:	
	void Initialize(ABuildingConstruction* ConstructionBase);
	void StopBuilding();
	void StartBuilding();
	void OnBuildingCompleted();
	void StopGoingTo();
};
