#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "UI/VillagerPanel.h"
#include "Actors/Units/MoveableUnit.h"
#include "RTSGameMode.generated.h"

class FHarvestingManager;
class FCollisionManager;
class FObjectsManager;
class FMapGenerator;
class FUpgradeManager;
class FDropoffsManager;
UCLASS()
class AOE2_API ARTSGameMode : public AGameMode
{
	GENERATED_BODY()
	ARTSGameMode();
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void BeginDestroy() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
protected:
	FObjectsManager* ObjectsManager;
	const FMapGenerator* MapGenerator;
	FUpgradeManager* UpgradeManager;
	FHarvestingManager* HarvestingManager;
	FDropoffsManager* DropoffsManager;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* Units;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* Buildings;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* Upgrades;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* ResourcesInfo;
	bool BGameModeLoaded = false;
	/**
	 * It needs to be set by the map generator or from a menu. It's hardcoded for now since I don't have any dynamic maps.
	 */
	const uint8 GridSize = 100;
	const uint16 CellSize = 100;
	const uint32 MapSize = 20000;
public:
	FObjectsManager* GetObjectManager() const;
	FUpgradeManager* GetUpgradeManager() const;
	FDropoffsManager* GetDropoffManager();
	FHarvestingManager* GetHarvestingManager();
	void LoadDefaultActors(ARTSHUD* PlayerHUD);
	uint8 GetGridSize() const;
	uint16 GetCellSize() const;
	uint32 GetMapSize() const;
	void UnitMoved(const AMoveableUnit* Unit, const FVector2d& NewLocation);
};
