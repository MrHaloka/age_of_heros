#include "Components/LineOfSightComponent.h"

#include "Iterators/GridInLineIterator.h"
#include "Managers/ObjectsManager.h"

FLineOfSightComponent::FLineOfSightComponent(FObjectsManager* ObjectsManager) :
ObjectsManager(ObjectsManager)
{
}

TArray<FVector2d> FLineOfSightComponent::GetBlockingObjects(FGridInLineIterator GridIterator) const
{
	TSet<FVector2d> Blocked;
	while (!GridIterator.IsFinish())
	{
		FVector2d CurrentGrid = GridIterator.Iterate();
		if (ObjectsManager->IsGridBlock(ObjectsManager->GetGridId(CurrentGrid)))
		{
			Blocked.Add(CurrentGrid);
		}
	}
	return Blocked.Array();
}

bool FLineOfSightComponent::HasLineOfSight(FGridInLineIterator GridIterator) const
{
	while (!GridIterator.IsFinish())
	{
		FVector2d CurrentGrid = GridIterator.Iterate();
		if (ObjectsManager->IsGridBlock(ObjectsManager->GetGridId(CurrentGrid)))
		{
			return false;
		}
	}
	return true;
}
