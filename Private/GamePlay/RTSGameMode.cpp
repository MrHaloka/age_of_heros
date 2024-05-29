#include "GamePlay/RTSGameMode.h"

#include "Actors/Resources/BaseResources.h"
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
	ObjectsManager = MakeUnique<FObjectsManager>(GetWorld(), GridSize, CellSize);
	UpgradeManager = MakeUnique<FUpgradeManager>();
	GameStatics::Initialize(*Units, *Buildings, *Upgrades, *ResourcesInfo, *ProjectilesInfo);
	MapGenerator =  MakeUnique<FMapGenerator>(*ObjectsManager);
	HarvestingManager = MakeUnique<FHarvestingManager>(ObjectsManager.Get());
	DropoffsManager = MakeUnique<FDropoffsManager>(ObjectsManager.Get());
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
	return ObjectsManager.Get();
}

FUpgradeManager* ARTSGameMode::GetUpgradeManager() const
{
	return UpgradeManager.Get();
}

FDropoffsManager* ARTSGameMode::GetDropoffManager()
{
	return DropoffsManager.Get();
}

FHarvestingManager* ARTSGameMode::GetHarvestingManager()
{
	return HarvestingManager.Get();
}

void ARTSGameMode::LoadDefaultActors(ARTSHUD* PlayerHUD)
{
	MapGenerator->AddAllDefaultsToMiniMap(PlayerHUD);
}

uint8 ARTSGameMode::GetGridSize() const
{
	return GridSize;
}

uint16 ARTSGameMode::GetCellSize() const
{
	return CellSize;
}

uint32 ARTSGameMode::GetMapSize() const
{
	return MapSize;
}

void ARTSGameMode::UnitMoved(const AMoveableUnit* Unit, const FVector2d& NewLocation)
{
	AActor* ActorInGrid = ObjectsManager->GetActorInGrid(Unit->GetActorLocation2d());
	ARTSHUD* HUD = GetWorld()->GetFirstPlayerController()->GetPawn<APlayerSpectatorPawn>()->GetRTSHUD();
	if (ActorInGrid == nullptr)
	{
		HUD->UnitMoved(Unit->GetID(),Unit->GetActorLocation2d(), NewLocation);
	}
	else if(ABaseResources* Resource = Cast<ABaseResources>(ActorInGrid))
	{
		HUD->UnitMoved(Unit->GetID(),Unit->GetActorLocation2d(), NewLocation, Resource->GetMiniMapColor());
	}
	else
	{
		HUD->UnitMoved(Unit->GetID(),Unit->GetActorLocation2d(), NewLocation, FColor::Blue);
	}
}
