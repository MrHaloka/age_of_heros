#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Actors/Units/Buildings/BaseBuilding.h"
#include "UnitControllerComponent.generated.h"

class FEmptyLocationHandler;
class APlayerSpectatorPawn;
class FObjectsManager;
UCLASS(ClassGroup=(Custom))
class AOE2_API UUnitControllerComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UUnitControllerComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void NewLocationSelected(const FVector2d& Location);
	void UnselectUnits();
protected:
	void CreateHandlers();
	virtual void BeginPlay() override;
	FObjectsManager* ObjectsManager;
	UPROPERTY()
	TSet<ABaseUnit*> SelectedUnits;
	UPROPERTY()
	APlayerSpectatorPawn* Player;
	TUniquePtr<FEmptyLocationHandler> InputChainHandler;
};
