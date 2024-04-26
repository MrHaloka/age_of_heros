#pragma once

#include "CoreMinimal.h"

class FSpatialHash2d;
class ABaseUnit;
class UDropoffComponent;
enum class EResource;
class FObjectsManager;
/**
 * 
 */
class AOE2_API FDropoffsManager
{
public:
	FDropoffsManager(FObjectsManager* ObjectsManager);
	~FDropoffsManager();
	void AddNewDropoff(UDropoffComponent* Dropoff);
	void RemoveDropoff(UDropoffComponent* Dropoff);
	ABaseUnit* GetClosestDropOff(const FVector2d& Location, EResource ResourceType);
protected:
	TUniquePtr<FSpatialHash2d> HashGrid;
	FObjectsManager* ObjectsManager;
	TMap<uint32, UDropoffComponent*> Dropoffs = TMap<uint32, UDropoffComponent*>();
	uint32 CurrentID = 1;
};
