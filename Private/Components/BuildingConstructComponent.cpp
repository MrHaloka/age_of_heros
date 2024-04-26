#include "Components/BuildingConstructComponent.h"

#include "DataStructures/BuildingInfo.h"
#include "GamePlay/GameStatics.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/ObjectsManager.h"
#include "Actors/Units/BuildingConstruction.h"

FBuildingConstructComponent::FBuildingConstructComponent(FObjectsManager* ObjectsManager):
ObjectsManager(ObjectsManager)
{
}

bool FBuildingConstructComponent::CanPlaceOn(const FBuildingInfo* BuildingInfo, const FVector2d Location) const
{
	uint16 CellSize = ObjectsManager->GetCellSize();
	for (int X = 0; X < BuildingInfo->SizeX ; X+= CellSize)
	{
		for (int Y = 0; Y < BuildingInfo->SizeY; Y+= CellSize)
		{
			if (ObjectsManager->IsGridBlock(Location + FVector2d(X, Y), true))
			{
				return false;
			}
		}
	}
	return true;
}

bool FBuildingConstructComponent::CanStartBuilding(const FBuildingInfo* BuildingInfo, const FVector2d Location, uint32 BuildingId) const
{
	uint16 CellSize = ObjectsManager->GetCellSize();
	for (int X = 0; X < BuildingInfo->SizeX ; X+= CellSize)
	{
		for (int Y = 0; Y < BuildingInfo->SizeY; Y+= CellSize)
		{
			if (ObjectsManager->IsUnitInGrid(Location + FVector2d(X, Y), BuildingId))
			{
				return false;
			}
		}
	}
	return true;
}

ABaseUnit* FBuildingConstructComponent::Build(UWorld* World, const FVector& Location, const FBuildingInfo* BuildingInfo, ETeams PlayerTeam)
{
	FTransform ActorTransform = FTransform(FRotator::ZeroRotator,Location);
	ABuildingConstruction* BuildingConstruction = World->SpawnActorDeferred<ABuildingConstruction>(BuildingInfo->ConstructionClass, ActorTransform);
	BuildingConstruction->Initialize(BuildingInfo, PlayerTeam);
	ABaseUnit* Unit = CastChecked<ABaseUnit>(UGameplayStatics::FinishSpawningActor(BuildingConstruction, ActorTransform));
	GameStatics::GetObjectManager()->AddUnitToHash(*Unit, BuildingInfo->GetSize(), true);
	return Unit;
}
