#pragma once

#include "CoreMinimal.h"
#include "MoveableUnit.h"
#include "DataStructures/Inventory.h"
#include "Enums/UnitState.h"
#include "Villager.generated.h"

class UResourceHarvestingComponent;
class UDropoffComponent;
struct FInventory;
class ABaseBuilding;
class ABaseResources;
class ABuildingConstruction;
/**
 * 
 */
UCLASS()
class AVillager : public AMoveableUnit
{
	GENERATED_BODY()
public:
	AVillager();
	virtual void BeginPlay() override;
	void GatherResource(ABaseResources* Resource);
	void SetDropOffPoint(UDropoffComponent* NewDropOff);
	virtual void ShowUnitPanel(ARTSHUD* RTSHUD) override;
	void GoToConstructionSite(ABuildingConstruction* ConstructionBase);
	void AddToInventory(const FSingleResource& NewResource);
	const FInventory& GetInventory() const;
	void GoBackToDropoff();
protected:
	void ReachedDropoff();
	virtual void OnFinalPathfindingGoalReached() override;
	UPROPERTY()
	UDropoffComponent* DropOffPoint;
	FInventory Inventory = FInventory(2);
};
