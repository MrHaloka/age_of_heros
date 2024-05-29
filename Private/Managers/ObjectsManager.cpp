#include "Managers/ObjectsManager.h"

#include "Containers/SpatialHash2d.h"
#include "DataStructures/ResourcesInfo.h"
#include "Factories/UnitInfoFactory.h"
#include "GamePlay/RTSGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/HarvestingManager.h"
#include "Actors/Units/BaseUnit.h"
#include "Actors/Units/BuildingConstruction.h"
#include "Actors/Units/MoveableUnit.h"
#include "Actors/Units/Buildings/BaseBuilding.h"
#include "Actors/Resources/BaseResources.h"
#include "GamePlay/GameStatics.h"
#include "Managers/PerceptionManager.h"

FObjectsManager::FObjectsManager(UWorld* NewWorld, uint8 GridSize, uint16 CellSize) : World(NewWorld)
{
	HashGrid = MakeUnique<FSpatialHash2d>(GridSize, CellSize);
	PerceptionManager = MakeUnique<FPerceptionManager>(this, GridSize, CellSize);
}

FObjectsManager::~FObjectsManager()
{
}

uint32 FObjectsManager::GetGridId(const FVector2d& GridLocation) const
{
	return HashGrid->ToGrid(GridLocation);
}

FVector2d FObjectsManager::GetGridFrom(uint32 Id) const
{
	return HashGrid->FromGrid(Id);
}

ABaseUnit* FObjectsManager::SpawnUnit(const FVector& Location, const FUnitInfoFactory& UnitInfo, ETeams Team)
{
	ABaseUnit* NewUnit = World->SpawnActor<ABaseUnit>(UnitInfo.GetUnitInfo(), Location, FRotator::ZeroRotator);
	NewUnit->SetTeam(Team);
	AddUnitToHash(*NewUnit, UnitInfo.GetUnitSize());
	return NewUnit;
}

void FObjectsManager::AddUnitToHash(ABaseUnit& Unit, const FVector2d& UnitSize, bool BIsHologram)
{
	Unit.SetID(CurrentId);
	Units.Add(CurrentId, &Unit);
	UnitSize.IsZero() ? HashGrid->InsertPoint(CurrentId, Unit.GetActorLocation2d()) : HashGrid->InsertPoints(CurrentId, Unit.GetActorLocation2d(), UnitSize);
	if (BIsHologram)
	{
		Holograms.Add(CurrentId);
	}
	CurrentId += 1;
	
	PerceptionManager->UnitAddedToGridListener(&Unit, true);
	OnUnitAddedToGridEvent.Broadcast(&Unit);
}

ABaseResources* FObjectsManager::SpawnResource(const FVector& Location, TSubclassOf<ABaseResources> ResourceClass)
{
	ABaseResources* NewResource = World->SpawnActor<ABaseResources>(ResourceClass, Location, FRotator::ZeroRotator);
	AddResourceToHash(*NewResource);
	return NewResource;
}

void FObjectsManager::AddResourceToHash(ABaseResources& Resource)
{
	Resources.Add(CurrentId, &Resource);
	HashGrid->InsertPoints(CurrentId, Resource.GetActorLocation2d(), FVector2d(100, 100));
	Resource.SetID(CurrentId);
	CurrentId += 1;
}

ABaseUnit* FObjectsManager::GetUnitInLocation(const FVector2d& Location, int8 Radius)
{
	TOptional<uint32> UnitID = HashGrid->FindNearestInRadius(
		Location,
		Radius,
		[&Location , &Radius, this](const uint32& ID)-> float
		{
			if (Units.FindChecked(ID)->IsA(ABaseBuilding::StaticClass()) || Units.FindChecked(ID)->IsA(ABuildingConstruction::StaticClass()))
			{
				return Radius * Radius - 1;
			}
			return (FVector2d(Units.FindChecked(ID)->GetActorLocation()) - Location).SizeSquared();
		}, [this](const uint32& ID)
		{
			return Resources.Contains((ID));
		});
	return UnitID.IsSet() ? Units.FindChecked(UnitID.GetValue()) : nullptr;
}

TSet<FVector2d> FObjectsManager::GetAllUnblockedAdjacentNeighbours(const FVector2d& GridLocation)
{
	TSet<FVector2d> Locations;
	TSet<uint32> NeighboursIdS = HashGrid->GetAllAdjacentNeighboursIds(GridLocation);
	for (uint32 NeighbourId : NeighboursIdS)
	{
		if (!IsGridBlock(NeighbourId))
		{
			Locations.Add(HashGrid->FromGrid(NeighbourId));
		}
	}
	return Locations;
}

TArray<uint32> FObjectsManager::GetAllUnblockedAdjacentNeighboursIds(const FVector2d& GridLocation)
{
	TSet<uint32> NeighboursIdS = HashGrid->GetAllAdjacentNeighboursIds(GridLocation);
	TArray<uint32> Results;
	for (uint32 NeighbourId : NeighboursIdS)
	{
		if (!IsGridBlock(NeighbourId))
		{
			Results.Add(NeighbourId);
		}
	}
	return Results;
}

bool FObjectsManager::IsGridBlock(uint32 GridId, bool BIsHologramCount)
{
	return HashGrid->GetPointInGrid(
		GridId, [this, BIsHologramCount](const uint32& ID)-> bool
		{
			if (Resources.Contains(ID))
			{
				return true;
			}
			if (Holograms.Contains(ID))
			{
				return BIsHologramCount;
			}
			return Units.FindChecked(ID)->IsA(ABaseBuilding::StaticClass()) || Units.FindChecked(ID)->IsA(
				ABuildingConstruction::StaticClass());
		}).IsSet();
}

bool FObjectsManager::IsGridBlock(FVector2d GridLocation, bool BIsHologramCount)
{
	return IsGridBlock(GetGridId(GridLocation), BIsHologramCount);
}

TSet<FVector2d> FObjectsManager::GetAllBlockedNeighbours(const FVector2d& GridLocation)
{
	TSet<FVector2d> Neighbours;
	TSet<uint32> NeighboursIds = HashGrid->GetAllNeighboursIds(GridLocation);
	for (uint32 NeighbourId : NeighboursIds)
	{
		if (IsGridBlock(NeighbourId))
		{
			Neighbours.Add(HashGrid->FromGrid(NeighbourId));
		}
	}
	return Neighbours;
}

AActor* FObjectsManager::GetActor(uint32 ActorId)
{
	if (Units.Contains(ActorId))
	{
		return Units.FindChecked(ActorId);
	}
	return Resources.FindChecked(ActorId);
}

AActor* FObjectsManager::GetActorInLocation(const FVector2d& Location, int8 Radius, uint32 IgnoreId)
{
	int32 MaxRadius = Radius * Radius - 1;
	TOptional<uint32> ActorId = HashGrid->FindNearestInRadius(
		Location,
		Radius,
		[&Location , &MaxRadius, this](const uint32& ID)-> float
		{
			if (Resources.Contains(ID))
			{
				return MaxRadius;
			}
			// if it's not resources then it should be in units
			if (Units.FindChecked(ID)->IsA(ABaseBuilding::StaticClass()) || Units.FindChecked(ID)->IsA(
				ABuildingConstruction::StaticClass()) || Holograms.Contains(ID))
			{
				return MaxRadius;
			}
			return FVector2d::DistSquared(Units.FindChecked(ID)->GetActorLocation2d(), Location);
		},
		[&IgnoreId](const uint32& ID)->bool
		{
			return IgnoreId == ID;
		});
	return ActorId.IsSet() ? GetActor(ActorId.GetValue()) : nullptr;
}

AActor* FObjectsManager::GetActorInLocation(const FVector2d& Location, ETeams IgnoreTeam, int8 Radius)
{
	int32 MaxRadius = Radius * Radius - 1;
	TOptional<uint32> ActorId = HashGrid->FindNearestInRadius(
		Location,
		Radius,
		[&Location , &MaxRadius, this](const uint32& ID)-> float
		{
			if (Units.FindChecked(ID)->IsA(ABaseBuilding::StaticClass()) || Units.FindChecked(ID)->IsA(ABuildingConstruction::StaticClass()))
			{
				return MaxRadius;
			}
			return FVector2d::DistSquared(Units.FindChecked(ID)->GetActorLocation2d(), Location);
		},
		[this, &IgnoreTeam](const uint32& ID)->bool
		{
			if (Resources.Contains(ID) || Holograms.Contains(ID))
			{
				return true;
			}
			return Units.FindChecked(ID)->GetTeam() == IgnoreTeam;
		});
	return ActorId.IsSet() ? GetActor(ActorId.GetValue()) : nullptr;
}

void FObjectsManager::RemoveUnitUnsafe(uint32 ID, FVector2d Location)
{
	if (Units.FindChecked(ID)->GetSize().IsZero())
	{
		HashGrid->RemovePointUnsafe(ID, Location);
	}
	else
	{
		HashGrid->RemovePointsUnsafe(ID, Location, Units.FindChecked(ID)->GetSize());
	}
	Units.Remove(ID);
}

void FObjectsManager::RemoveResourceUnsafe(uint32 ResouceId, const FVector2d Location)
{
	HashGrid->RemovePointUnsafe(ResouceId, Location);
	Resources.Remove(ResouceId);
	if (ARTSGameMode* GameMode = Cast<ARTSGameMode>(UGameplayStatics::GetGameMode(GEngine->GetCurrentPlayWorld())))
	{
		return GameMode->GetHarvestingManager()->ResourceDestroyed(ResouceId);
	}
}

void FObjectsManager::RemoveHologram(uint32 HologramId)
{
	Holograms.Remove(HologramId);
}

ABaseResources* FObjectsManager::GetResourceInGrid(uint32 GridId)
{
	TOptional<uint32> ResourceId = HashGrid->GetPointInGrid(
		GridId, [this](const uint32& ID)-> bool
		{
			if (Resources.Contains(ID))
			{
				return true;
			}
			return false;
		});
	return ResourceId.IsSet() ? Resources.FindChecked(ResourceId.GetValue()) : nullptr;
}

uint16 FObjectsManager::GetCellSize() const
{
	return HashGrid->GetCellSize();
}

TSet<ABaseResources*> FObjectsManager::GetAdjacentResource(const FVector2d& GridLocation)
{
	TSet<ABaseResources*> NeighbourResources;
	TSet<uint32> NeighboursIdS = HashGrid->GetAllAdjacentNeighboursIds(GridLocation);
	for (uint32 NeighbourId : NeighboursIdS)
	{
		if (ABaseResources* Resource = GetResourceInGrid(NeighbourId))
		{
			NeighbourResources.Add(Resource);
		}
	}
	return NeighbourResources;
}

ABaseResources* FObjectsManager::GetResourceNear(FVector2d Location, EResource ResourceType, int32 Radius)
{
	TOptional<uint32> ResourceId = HashGrid->FindNearestInRadius(
		Location,
		Radius,
		[&Location ,this](const uint32& ID)-> float
		{
			return FVector2d::DistSquared((*Resources.Find(ID))->GetActorLocation2d(), Location);
		}, [&ResourceType, this](const uint32& ID)-> bool
		{
			return !Resources.Contains(ID) || (*Resources.Find(ID))->GetResourceInfo()->Type != ResourceType;
		});
	return ResourceId.IsSet() ? *Resources.Find(ResourceId.GetValue()) : nullptr;
}

bool FObjectsManager::IsBlock(const FVector2d& Location, float Radius, const uint32 IgnoreID)
{
	return HashGrid->IsAnyInRadius(
		Location,
		Radius,
		[&Location, this](const uint32& ID)-> float
		{
			if (Resources.Contains(ID) || Units.FindChecked(ID)->IsA(ABaseBuilding::StaticClass()) || Units.FindChecked(ID)->IsA(ABuildingConstruction::StaticClass()))
			{
				return 0;
			}
			return FVector2d::DistSquared(Units.FindChecked(ID)->GetActorLocation2d(), Location);
		},
		[&IgnoreID, this](const uint32& ID)-> bool { return ID == IgnoreID || Holograms.Contains(ID); }
	);
}

void FObjectsManager::UnitMoved(AMoveableUnit* Unit, const FVector2d& NewLocation)
{
	if(HashGrid->UpdatePoint(Unit->GetID(), Unit->GetActorLocation2d(), NewLocation))
	{
		GameStatics::GetRTSGameMode()->UnitMoved(&*Unit, NewLocation);
		OnUnitMovedoutGridEvent.Broadcast(Unit, NewLocation);
	}
	PerceptionManager->UnitMovedListener(Unit, NewLocation);
}

AActor* FObjectsManager::GetActorInGrid(const FVector2d& GridLocation)
{
	TOptional<uint32> PointId = HashGrid->GetPointInGrid(GridLocation, [](const uint32& ID)-> bool{return true;});
	if (PointId.IsSet())
	{
		if (Units.Contains(PointId.GetValue()))
		{
			return Units.FindChecked(PointId.GetValue());
		}
		return Resources.FindChecked(PointId.GetValue());
	}
	return nullptr;
}
bool FObjectsManager::IsUnitInGrid(const FVector2d& GridLocation, uint32 IgnoreID) const
{
	return HashGrid->GetPointInGrid(
		GridLocation, [&IgnoreID, this](const uint32& ID)-> bool
		{
			return ID != IgnoreID && Units.Contains(ID);
		}).IsSet();
}

TSet<uint32> FObjectsManager::GetNeighboursMoveable(const FVector2d& Location, uint32 TargetId, int Radius)
{
	return HashGrid->FindAllInRadius(
		Location,
		Radius,
		[&Location, this](const uint32& ID)-> float
		{
			return (FVector2d(Units.FindChecked(ID)->GetActorLocation()) - Location).SizeSquared();
		}, [this, TargetId](const uint32& ID)-> bool
		{
			return TargetId == ID || Resources.Contains(ID) || !Units.FindChecked(ID)->IsA(AMoveableUnit::StaticClass());
		});
}

const TMap<int32, ABaseUnit*>& FObjectsManager::GetUnits()
{
	return Units;
}

/**
 * Retrieves the set of movable units that are inside the specified rectangle.
 *
 * @param MaxPoint the maximum x and y coordinates of the rectangle.
 * @param MinPoint the minimum x and y coordinates of the rectangle.
 * @return the set of movable units inside the specified rectangle.
 */
TSet<ABaseUnit*> FObjectsManager::GetMoveableInsideRectangle(const FVector2d& MaxPoint, const FVector2d& MinPoint)
{
	TSet<uint32> MoveableIds = HashGrid->FindAllInRectangle(
		MaxPoint,
		MinPoint,
		[this, &MaxPoint, &MinPoint](const uint32& ID)-> bool
	   {
		   return Units.FindChecked(ID)->GetActorLocation2d().ComponentwiseAllGreaterOrEqual(MinPoint) &&
			Units.FindChecked(ID)->GetActorLocation2d().ComponentwiseAllLessOrEqual(MaxPoint);
	   },
	   [this](const uint32& ID)-> bool
	   {
		   return  !Units.Contains(ID) || !Units.FindChecked(ID)->IsA(AMoveableUnit::StaticClass());
	   });
	TSet<ABaseUnit*> Result;
	for (const uint32& UnitId : MoveableIds)
	{
		Result.Add(Units.FindChecked(UnitId));
	}
	return Result;
}

TUniquePtr<FPerceptionManager>& FObjectsManager::GetPerceptionManager()
{
	return PerceptionManager;
}

TArray<uint32> FObjectsManager::GetActorsInGrid(const FVector2d& GridLocation)
{
	return HashGrid->GetPointsInGrid(GridLocation);
}

TSet<uint32> FObjectsManager::GetUnitsInLocation(const FVector2d& Location, uint32 Radius, uint32 IgnoreId)
{
	return HashGrid->FindAllInRadius(
		Location,
		Radius,
		[&Location, this](const uint32& ID)-> float
		{
			if (Units.FindChecked(ID)->IsA(ABaseBuilding::StaticClass()) || Units.FindChecked(ID)->IsA(ABuildingConstruction::StaticClass()))
			{
				return 0;
			}
			return FVector2d::DistSquared(Units.FindChecked(ID)->GetActorLocation2d(), Location);
		},
		[&IgnoreId, this](const uint32& ID)-> bool { return ID == IgnoreId || Holograms.Contains(ID) || Resources.Contains(ID); }
	);
}
