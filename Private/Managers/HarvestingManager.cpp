#include "Managers/HarvestingManager.h"

#include <queue>

#include "Helpers/GridHelper.h"
#include "Iterators/GridInLineIterator.h"
#include "Actors/Resources/BaseResources.h"
#include "Managers/ObjectsManager.h"

FHarvestingManager::FHarvestingManager(FObjectsManager* ObjectsManager) : ObjectsManager(ObjectsManager)
{
}

TArray<FVector2d> FHarvestingManager::GetSlotFromNeighbours(TArray<FVector2d>& Neighbours, const FVector2d& ResourceLocation)
{
	TArray<FVector2d> Slots;
	for (FVector2d& Neighbour : Neighbours)
	{
		FVector2d Offset = FVector2d::Zero();
		if (Neighbour.X == ResourceLocation.X)
		{
			FVector2d XOffset = FVector2d(20, 0);
			Offset = FVector2d(0, 50);
			Slots.Add(Neighbour + Offset + XOffset);
			Slots.Add(Neighbour + Offset + FVector2d(100, 0) - XOffset);
			continue;
		}
		if (Neighbour.Y == ResourceLocation.Y)
		{
			FVector2d YOffset = FVector2d(0, 20);
			Offset = FVector2d(50, 0);
			Slots.Add(Neighbour + Offset + YOffset);
			Slots.Add(Neighbour + Offset + FVector2d(0, 100) - YOffset);
		}
	}
	return Slots;
}

void FHarvestingManager::AddNewResourceToList(const FVector2d& VillagerLocation, ABaseResources*& Resource)
{
	TArray<FVector2d> Neighbours = ObjectsManager->GetAllUnblockedAdjacentNeighbours(Resource->GetActorLocation2d()).Array();
	if (Neighbours.IsEmpty())
	{
		FindTheClosestAvailableResource(VillagerLocation, Resource);
		if (Resources.Contains(Resource->GetID()))
		{
			return;
		}
		Neighbours = ObjectsManager->GetAllUnblockedAdjacentNeighbours(GridHelper::GetGridFromVector(Resource->GetActorLocation())).Array();
	}
	Resources.Add(Resource->GetID(), GetSlotFromNeighbours(Neighbours, Resource->GetActorLocation2d()));
}

void FHarvestingManager::AddNewResourceToListIfPossible(ABaseResources*& Resource)
{
	TArray<FVector2d> Neighbours = ObjectsManager->GetAllUnblockedAdjacentNeighbours(Resource->GetActorLocation2d()).Array();
	if (Neighbours.IsEmpty())
	{
		return;
	}
	Resources.Add(Resource->GetID(), GetSlotFromNeighbours(Neighbours, FVector2d(Resource->GetActorLocation())));
}

void FHarvestingManager::FindTheClosestAvailableResource(const FVector2d& VillagerLocation, ABaseResources*& Resource)
{
	FGridInLineIterator GridIterator(Resource->GetActorLocation2d(), VillagerLocation, ObjectsManager->GetCellSize());
	while (!GridIterator.IsFinish())
	{
		uint32 CurrentGridId = ObjectsManager->GetGridId(GridIterator.Iterate());
		if (ObjectsManager->IsGridBlock(CurrentGridId))
		{
			Resource = ObjectsManager->GetResourceInGrid(CurrentGridId);
			continue;
		}
		return;
	}
}

void FHarvestingManager::AddSlotBackToResource(uint32 ResourceID, FVector2d Location)
{
	Resources.Find(ResourceID)->Add(Location);
}

FVector2d FHarvestingManager::GetResourceLocation(const FVector2d& VillagerLocation, ABaseResources*& Resource)
{
	if (!Resources.Contains(Resource->GetID()))
	{
		AddNewResourceToList(VillagerLocation, Resource);
	}
	if (Resources.Find(Resource->GetID())->IsEmpty())
	{
		SetToClosestResourceNeighbour(VillagerLocation, Resource);
	}
	Resources.Find(Resource->GetID())->Sort([&VillagerLocation](const FVector2d& LHS, const FVector2d& RHS)
	{
		return FVector2d::DistSquared(LHS, VillagerLocation) > FVector2d::DistSquared(RHS, VillagerLocation);
	});
	if (Resources.Find(Resource->GetID())->IsEmpty())
	{
		if (Resource != nullptr)
		{
			DrawDebugBox(GEngine->GetCurrentPlayWorld(), Resource->GetActorLocation(), FVector(20, 20, 200), FColor::Purple, true, 1, 0, 10);
		}
		return FVector2d::Zero();
	}
	return Resources.Find(Resource->GetID())->Pop();
}

ABaseResources* FHarvestingManager::GetNewResource(const FVector2d& VillagerLocation, EResource ResourceType) const
{
	return ObjectsManager->GetResourceNear(VillagerLocation, ResourceType, 500);
}

void FHarvestingManager::ResourceDestroyed(uint32 ResourceID)
{
	if (Resources.Contains(ResourceID))
	{
		Resources.Remove(ResourceID);
	}
}

void FHarvestingManager::VillagerStoppedHarvesting(uint32 ResourceID, FVector2d HarvestSlot)
{
	if(Resources.Contains(ResourceID))
	{
		Resources.Find(ResourceID)->Add(HarvestSlot);
	}
}

void FHarvestingManager::SetToClosestResourceNeighbour(const FVector2d& VillagerLocation, ABaseResources*& Resource)
{
	std::priority_queue<std::pair<float, ABaseResources*>, std::vector<std::pair<float, ABaseResources*>>, std::greater<std::pair<float, ABaseResources*>>> OpenNeighbours;
	TSet<uint32> CloseIDs;
	int32 DebugCounter = 0;
	OpenNeighbours.push(std::pair(FVector2d::DistSquared(VillagerLocation, FVector2d(Resource->GetActorLocation())),Resource));
	CloseIDs.Add(Resource->GetID());
	while (!OpenNeighbours.empty() && DebugCounter < 200)
	{
		ABaseResources* LastResource = OpenNeighbours.top().second;
		OpenNeighbours.pop();
		TSet<ABaseResources*> NeighboutResourceses = ObjectsManager->GetAdjacentResource(LastResource->GetActorLocation2d());
		for (ABaseResources* Neighbour : NeighboutResourceses)
		{
			if (Neighbour == nullptr || CloseIDs.Contains(Neighbour->GetID()))
			{
				continue;
			}
			if (!Resources.Contains(Neighbour->GetID()))
			{
				AddNewResourceToListIfPossible(Neighbour);
			}
			if (Resources.Contains(Neighbour->GetID()) && !Resources.Find(Neighbour->GetID())->IsEmpty())
			{
				Resource = &*Neighbour;
				return;
			}
			OpenNeighbours.push(std::pair(FVector2d::DistSquared(VillagerLocation, FVector2d(Neighbour->GetActorLocation())),Neighbour));
			CloseIDs.Add(Neighbour->GetID());
		}
		DebugCounter++;
	}
}
