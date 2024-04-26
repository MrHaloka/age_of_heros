#pragma once

#include "CoreMinimal.h"
#include "Chaos/SpatialHash.h"
#include "Enums/Resource.h"

class FUnitInfoFactory;
class FSpatialHash2d;
class ABaseBuilding;
struct FBuildingInfo;
struct FResourcesInfo;
class ABaseResources;
class AMoveableUnit;
struct FUnitInfo;
enum class ETeams;
class ABaseUnit;

class AOE2_API FObjectsManager
{
public:
	FObjectsManager(UWorld* NewWorld, uint8 GridSize, uint16 CellSize);
	~FObjectsManager();
	ABaseUnit* SpawnUnit(const FVector& Location,const FUnitInfoFactory& UnitInfo, ETeams ETeam);
	ABaseUnit* GetUnitInLocation(const FVector2d& Location, int8 Radius = 6);
	TSet<FVector2d> GetAllUnblockedAdjacentNeighbours(const FVector2d& GridLocation);
	TArray<uint32> GetAllUnblockedAdjacentNeighboursIds(const FVector2d& GridLocation);
	bool IsBlock(const FVector2d& Location, float Radius, const uint32 IgnoreID = 0);
	bool UnitMoved(const AMoveableUnit* Unit, const FVector2d& NewLocation);
	bool IsUnitInGrid(const FVector2d& Vector2, uint32 IgnoreID = 0) const;
	bool IsGridBlock(uint32 NeighbourId, bool BIsHologramCount = false);
	bool IsGridBlock(FVector2d GridLocation, bool BIsHologramCount = false);
	uint32 GetGridId(const FVector2d& GridLocation) const;
	FVector2d GetGridFrom(uint32 Id) const;
	TSet<FVector2d> GetAllBlockedNeighbours(const FVector2d& GridLocation);
	AActor* GetActorInLocation(const FVector2d& Location, int8 Radius = 25);
	void SpawnResource(const FVector& Location, TSubclassOf<ABaseResources> ResourceClass);
	void RemoveUnitUnsafe(uint32 ID, FVector2d Location);
    void RemoveResourceUnsafe(uint32 ResourceId, FVector2d Location);
	void RemoveHologram(uint32 HologramId);
	TSet<ABaseResources*> GetAdjacentResource(const FVector2d& GridLocation);
	ABaseResources* GetResourceNear(FVector2d Location, EResource ResourceType, int32 Radius = 200);
	ABaseResources* GetResourceInGrid(uint32 GridId);
	uint16 GetCellSize() const;
	void AddUnitToHash(ABaseUnit& Unit, const FVector2d& UnitSize, bool BIsHologram = false);
	TSet<uint32> GetNeighboursMoveable(const FVector2d& Location, uint32 TargetId, int Radius);
	const TMap<int32, ABaseUnit*>& GetUnits();
protected:
	TMap<int32, ABaseUnit*> Units;
	TMap<int32, ABaseResources*> Resources;
	TSet<int32> Holograms;
	TUniquePtr<FSpatialHash2d> HashGrid;
	int32 CurrentId = 0;
	UWorld* World;
	AActor* GetActor(uint32 ActorId);
	void AddResourceToHash(ABaseResources& Resource);
};
