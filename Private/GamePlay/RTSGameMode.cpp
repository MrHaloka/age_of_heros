#include "GamePlay/RTSGameMode.h"

#include "GamePlay/GameStatics.h"
#include "Managers/ObjectsManager.h"
#include "GamePlay/PlayerSpectatorPawn.h"
#include "GamePlay/RTSPlayerController.h"
#include "Managers/DropoffsManager.h"
#include "Managers/HarvestingManager.h"
#include "Managers/UpgradeManager.h"
#include "Map/MapGenerator.h"

ARTSGameMode::ARTSGameMode()
{
	PlayerControllerClass = ARTSPlayerController::StaticClass();
	DefaultPawnClass = APlayerSpectatorPawn::StaticClass();
}

void ARTSGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	ObjectsManager = new FObjectsManager(GetWorld(), GridSize, 100);
	UpgradeManager = new FUpgradeManager();
	GameStatics::Initialize(*Units, *Buildings, *Upgrades, *ResourcesInfo);
	MapGenerator = new FMapGenerator(*ObjectsManager);
	HarvestingManager = new FHarvestingManager(ObjectsManager);
	DropoffsManager = new FDropoffsManager(ObjectsManager);
}

void ARTSGameMode::BeginDestroy()
{
	Super::BeginDestroy();
	if (!HasActorBegunPlay())
	{
		return;
	}
	delete ObjectsManager;
	delete UpgradeManager;
	delete MapGenerator;
}

void ARTSGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ARTSGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}

FObjectsManager* ARTSGameMode::GetObjectManager() const
{
	return ObjectsManager;
}

FUpgradeManager* ARTSGameMode::GetUpgradeManager() const
{
	return UpgradeManager;
}

FDropoffsManager* ARTSGameMode::GetDropoffManager()
{
	return DropoffsManager;
}

FHarvestingManager* ARTSGameMode::GetHarvestingManager()
{
	return HarvestingManager;
}

uint16 ARTSGameMode::GetGridSize() const
{
	return GridSize;
}

uint16 ARTSGameMode::GetMapSize() const
{
	return MapSize;
}
