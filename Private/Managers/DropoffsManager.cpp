#include "Managers/DropoffsManager.h"
#include "Components/DropoffComponent.h"
#include "Containers/SpatialHash2d.h"
#include "Actors/Units/BaseUnit.h"

FDropoffsManager::FDropoffsManager(FObjectsManager* ObjectsManager) : ObjectsManager(ObjectsManager)
{
	HashGrid = MakeUnique<FSpatialHash2d>(20, 10);
}

FDropoffsManager::~FDropoffsManager()
{
}

void FDropoffsManager::AddNewDropoff(UDropoffComponent* Dropoff)
{
	Dropoff->SetID(CurrentID);
	Dropoffs.Add(CurrentID, Dropoff);
	HashGrid->InsertPoint(CurrentID, Dropoff->GetGridLocation());
	CurrentID++;
}

void FDropoffsManager::RemoveDropoff(UDropoffComponent* Dropoff)
{
	HashGrid->RemovePointUnsafe(Dropoff->GetID(), Dropoff->GetGridLocation());
	Dropoffs.Remove(Dropoff->GetID());
}

ABaseUnit* FDropoffsManager::GetClosestDropOff(const FVector2d& ActorLocation, EResource ResourceType)
{
	FVector2d GridLocation = FVector2d((FMath::Floor(ActorLocation.X / 100)), (FMath::Floor(ActorLocation.Y / 100)));
	TOptional<uint32> SearchResult = HashGrid->FindNearestInRadius(
		GridLocation,
		15,
		[&GridLocation,this](const int& ID)-> float
		{
			return FVector2d::DistSquared(GridLocation, (*Dropoffs.Find(ID))->GetGridLocation());
		}, [&ResourceType, this](const int& ID)
		{
			return (*Dropoffs.Find(ID))->GetResourceType() != ResourceType;
		});
	return SearchResult.IsSet()? (*Dropoffs.Find(SearchResult.GetValue()))->GetBaseUnitOwner() : nullptr;
}
