#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CursorSelectionComponent.generated.h"

class FObjectsManager;
class APlayerSpectatorPawn;
UCLASS( ClassGroup=(Custom) )
class AOE2_API UCursorSelectionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCursorSelectionComponent();
protected:
	virtual void BeginPlay() override;
	FVector2d LastStoredLocation;
public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	FVector2d PlayerClickedNewLocation();
	void StoreTheWorldLocation();
	FVector2d GetLastStoredLocation();
};
