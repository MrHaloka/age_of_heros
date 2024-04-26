#pragma once

#include "CoreMinimal.h"
#include "Actors/Units/Villager.h"

class FObjectsManager;
class ABaseResources;
/**
 * 
 */
class AOE2_API FHarvestingManager
{
public:
	FHarvestingManager(FObjectsManager* ObjectsManager);
	TArray<FVector2d> GetSlotFromNeighbours(TArray<FVector2d>& Neighbours, const FVector2d& ResourceLocation);
	void AddNewResourceToList(const FVector2d& VillagerLocation, ABaseResources*& Resource);
	void AddNewResourceToListIfPossible(ABaseResources*& Resource);
	void FindTheClosestAvailableResource(const FVector2d& VillagerLocation,ABaseResources*& Resource);
	void AddSlotBackToResource(uint32 ResourceID, FVector2d Location);
	FVector2d GetResourceLocation(const FVector2d& VillagerLocation, ABaseResources*& ResourceInOut);
	ABaseResources* GetNewResource(const FVector2d& VillagerLocation, EResource ResourceType) const;
	void ResourceDestroyed(uint32 ResourceID);
	void VillagerStoppedHarvesting(uint32 ResourceID, FVector2d HarvestSlot);
protected:
	TMap<uint32, TArray<FVector2d>> Resources = TMap<uint32, TArray<FVector2d>>();
	FObjectsManager* ObjectsManager;
	void SetToClosestResourceNeighbour(const FVector2d& VillagerLocation, ABaseResources*& ResourceInOut);
};
