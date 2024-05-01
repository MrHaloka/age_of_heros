#include "Map/MapGenerator.h"

#include "DataStructures/UnitInfo.h"
#include "Enums/Teams.h"
#include "Factories/UnitInfoFactory.h"
#include "GamePlay/GameStatics.h"
#include "Managers/ObjectsManager.h"
#include "Actors/Units/Buildings/BaseBuilding.h"
#include "Actors/Units/Buildings/House.h"
#include "Actors/Units/Buildings/TownCenter.h"
#include "DataStructures/ResourcesInfo.h"
#include "GamePlay/PlayerSpectatorPawn.h"


FMapGenerator::FMapGenerator(FObjectsManager& DefaultObjectsManager):
	ObjectsManager(DefaultObjectsManager)
{
	CreateTownCenter();
	CreateStartingVillagers();
	CreateResources();
	CreateHouses();
}

FMapGenerator::~FMapGenerator()
{
}

void FMapGenerator::AddAllDefaultsToMiniMap(ARTSHUD* RTSHUD) const
{
	for (AActor* Actor : DefaultActors)
	{
		if (ABaseResources* Resource = Cast<ABaseResources>(Actor))
		{
			RTSHUD->UnitAdded(FVector2d(Actor->GetActorLocation()), Resource->GetMiniMapColor());
			continue;
		}
		RTSHUD->UnitAdded(FVector2d(Actor->GetActorLocation()), FColor::Blue);
	}
}

void FMapGenerator::CreateTownCenter()
{
	FBuildingInfo* BuildingInfo = GameStatics::GetBuildingReferenceByID(1);
	DefaultActors.Add(&*ObjectsManager.SpawnUnit(FVector::Zero(),FUnitInfoFactory(BuildingInfo->UnitClass, FVector2d(BuildingInfo->SizeX, BuildingInfo->SizeY)) , ETeams::Blue));
}

void FMapGenerator::CreateStartingVillagers()
{
	FUnitInfo* UnitInfo = GameStatics::GetUnitReferenceByID(1);
	FUnitInfoFactory Villager(UnitInfo->UnitClass);
	DefaultActors.Add(ObjectsManager.SpawnUnit(FVector(600,200,0), Villager, ETeams::Blue));
	DefaultActors.Add(ObjectsManager.SpawnUnit(FVector(700,200,0), Villager, ETeams::Blue));
	DefaultActors.Add(ObjectsManager.SpawnUnit(FVector(800,200,0), Villager, ETeams::Blue));
	DefaultActors.Add(ObjectsManager.SpawnUnit(FVector(900,200,0), Villager, ETeams::Blue));
	DefaultActors.Add(ObjectsManager.SpawnUnit(FVector(600,2000,0), Villager, ETeams::Blue));
	DefaultActors.Add(ObjectsManager.SpawnUnit(FVector(700,2000,0), Villager, ETeams::Blue));
	DefaultActors.Add(ObjectsManager.SpawnUnit(FVector(800,2000,0), Villager, ETeams::Blue));
	DefaultActors.Add(ObjectsManager.SpawnUnit(FVector(900,2000,0), Villager, ETeams::Blue));
	DefaultActors.Add(ObjectsManager.SpawnUnit(FVector(300,700,0), Villager, ETeams::Blue));
}

void FMapGenerator::CreateResources()
{
	const FResourcesInfo* ResourcesInfo = GameStatics::GetResourceByID(1);
	for (int i = 0; i < 15; i ++)
	{
		for (int j = 0; j < 10; j ++)
		{
			DefaultActors.Add(ObjectsManager.SpawnResource(FVector(1000+ i * 100, 1000 + j * 100, 0), ResourcesInfo->ResourceClass));
		}
	}
	const FResourcesInfo* ResourcesInfo2 = GameStatics::GetResourceByID(2);
	for (int i = 0; i < 3; i ++)
	{
		for (int j = 0; j < 3; j ++)
		{
			DefaultActors.Add(ObjectsManager.SpawnResource(FVector(1000+ i * 100, j * 100, 0), ResourcesInfo2->ResourceClass));
		}
	}
	const FResourcesInfo* ResourcesInfo3 = GameStatics::GetResourceByID(3);
	for (int i = 0; i < 3; i ++)
	{
		for (int j = 0; j < 3; j ++)
		{
			DefaultActors.Add(ObjectsManager.SpawnResource(FVector( i * 100, 1000 +j * 100, 0), ResourcesInfo3->ResourceClass));
		}
	}
}

void FMapGenerator::CreateHouses()
{
	FBuildingInfo* BuildingInfo = GameStatics::GetBuildingReferenceByID(2);
	DefaultActors.Add(ObjectsManager.SpawnUnit(FVector(4000,0,0), FUnitInfoFactory(BuildingInfo->UnitClass, FVector2d(BuildingInfo->SizeX, BuildingInfo->SizeY)), ETeams::Blue));
	DefaultActors.Add(ObjectsManager.SpawnUnit(FVector(0,4000,0), FUnitInfoFactory(BuildingInfo->UnitClass, FVector2d(BuildingInfo->SizeX, BuildingInfo->SizeY)), ETeams::Blue));
}
