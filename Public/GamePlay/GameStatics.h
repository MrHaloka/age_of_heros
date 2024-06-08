// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"

struct FProjectileInfo;
class ARTSGameMode;
class FObjectsManager;
struct FBuildingInfo;
struct FResourcesInfo;
struct FTableRowBase;
struct FUpgrades;
class ABaseUnit;
struct FUnitInfo;
class UDataTable;
/**
 * 
 */
class AOE2_API GameStatics
{
public:
	GameStatics();
	~GameStatics();
	static FObjectsManager* GetObjectManager();
	static void LoadBuildings(const UDataTable& BuildingInfo);
	static void LoadProjectiles(const UDataTable& ProjectilesInfoInit);
	static void Initialize(const UDataTable& UnitInfoInit, const UDataTable& BuildingInfo, const UDataTable& UpgradeInfoInit, const UDataTable& ResourcesInfoInit, const UDataTable& ProjectilesInfoInit);
	static int32 GetClassTypeId(const FName& Name);
	static FUpgrades* GetUpgradeByID(const int32& ID);
	static FUnitInfo* GetUnitReferenceByID(const int32& ID);
	static FBuildingInfo* GetBuildingReferenceByID(const int32& ID);
	static FProjectileInfo* GetProjectileInfoByID(const int32& ID);
	static FProjectileInfo* GetProjectileInfoByClassName(const FName& ClassName);
	static const FResourcesInfo* GetResourceByID(const int32& ID);
	static void LoadUnits(const UDataTable& UnitInfoInit);
	static void LoadUpgrades(const UDataTable& UpgradeInfoInit);
	static void LoadResources(const UDataTable& DataTable);
	static ARTSGameMode* GetRTSGameMode();
	static TMap<int32, FBuildingInfo>& GetBuildings();
protected:
	inline static TMap<int32, FUpgrades> Upgrades = TMap<int32, FUpgrades>();
	inline static TMap<int32, FUnitInfo> Units = TMap<int32, FUnitInfo>();
	inline static TMap<int32, FBuildingInfo> Buildings = TMap<int32, FBuildingInfo>();
	inline static TMap<FName, int32> UnitClassIDS = TMap<FName, int32>();
	inline static TMap<FName, int32> BuildingClassIDS = TMap<FName, int32>();
	inline static TMap<FName, int32> ProjectileClassIDS = TMap<FName, int32>();
	inline static TMap<int32, FResourcesInfo> ResourcesInfo = TMap<int32, FResourcesInfo>();
	inline static TMap<int32, FProjectileInfo> Projectiles = TMap<int32, FProjectileInfo>();
};
