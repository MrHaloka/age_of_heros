#pragma once

#include "CoreMinimal.h"
#include "DataStructures/BuildingInfo.h"
#include "Actors/Units/BaseUnit.h"
#include "BaseBuilding.generated.h"

class AMoveableUnit;
class ARTSHUD;
UCLASS()
class AOE2_API ABaseBuilding : public ABaseUnit
{
	GENERATED_BODY()

public:
	ABaseBuilding();
	UStaticMeshComponent& GetMeshComponent();
	virtual FResources GetPrice() override;
	FVector2d GetClosestEmptySlot(const FVector2d& TargetLocation);
	virtual FVector2d GetSize() override;
protected:
	virtual float GetMaxHP() override;
	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* StaticMesh;
public:
	virtual void Tick(float DeltaTime) override;
	virtual void ShowUnitPanel(ARTSHUD* TargetHUD) override;
	const FBuildingInfo* GetBuildingInfo() const;
};
