#include "GamePlay/GameStatics.h"

#include "DataStructures/BuildingInfo.h"
#include "DataStructures/ResourcesInfo.h"
#include "DataStructures/UnitInfo.h"
#include "DataStructures/Upgrades.h"
#include "Engine/DataTable.h"
#include "GamePlay/RTSGameMode.h"
#include "Kismet/GameplayStatics.h"

GameStatics::GameStatics()
{
}

GameStatics::~GameStatics()
{
}

FObjectsManager* GameStatics::GetObjectManager()
{
	if (ARTSGameMode* GameMode = Cast<ARTSGameMode>(UGameplayStatics::GetGameMode(GEngine->GetCurrentPlayWorld())))
	{
		return GameMode->GetObjectManager();
	}
	return nullptr;
}

void GameStatics::LoadBuildings(const UDataTable& BuildingInfo)
{
	TArray<FName> Rows = BuildingInfo.GetRowNames();
	for (auto Row : Rows)
	{
		int ID = FCString::Atoi(*Row.ToString());
		FBuildingInfo Data = *BuildingInfo.FindRow<FBuildingInfo>(*FString::FromInt(ID), "ID", true);
		Buildings.Add(ID, Data);
		BuildingClassIDS.Add(Data.UnitClass->GetFName(), ID);
		BuildingClassIDS.Add(Data.ConstructionClass->GetFName(), ID);
	}
}

int32 GameStatics::GetClassTypeId(const FName& Name)
{
	if (UnitClassIDS.Contains(Name))
	{
		return *UnitClassIDS.Find(Name);
	}
	return *BuildingClassIDS.Find(Name);
}

void GameStatics::LoadUnits(const UDataTable& UnitInfoInit)
{
	TArray<FName> Rows = UnitInfoInit.GetRowNames();
	for (auto Row : Rows)
	{
		int ID = FCString::Atoi(*Row.ToString());
		FUnitInfo Data = *UnitInfoInit.FindRow<FUnitInfo>(*FString::FromInt(ID), "ID", true);
		Units.Add(ID, Data);
		UnitClassIDS.Add(Data.UnitClass->GetFName(), ID);
	}
}

void GameStatics::LoadUpgrades(const UDataTable& UpgradeInfoInit)
{
	TArray<FName> Rows = UpgradeInfoInit.GetRowNames();
	for (auto Row : Rows)
	{
		int ID = FCString::Atoi(*Row.ToString());
		FUpgrades Data = *UpgradeInfoInit.FindRow<FUpgrades>(*FString::FromInt(ID), "ID", true);
		Upgrades.Add(ID, Data);
	}
}

void GameStatics::LoadResources(const UDataTable& ResourceInfoInit)
{
	TArray<FName> Rows = ResourceInfoInit.GetRowNames();
	for (auto Row : Rows)
	{
		int ID = FCString::Atoi(*Row.ToString());
		FResourcesInfo Data = *ResourceInfoInit.FindRow<FResourcesInfo>(*FString::FromInt(ID), "ID", true);
		ResourcesInfo.Add(ID, Data);
	}
}

/**
 * Should only be called when RTSGameMode is running
 */
ARTSGameMode* GameStatics::GetRTSGameMode()
{
	return CastChecked<ARTSGameMode>(UGameplayStatics::GetGameMode(GEngine->GetCurrentPlayWorld()));
}

TMap<int32, FBuildingInfo>& GameStatics::GetBuildings()
{
	return Buildings;
}

FUpgrades* GameStatics::GetUpgradeByID(int32 ID)
{
	return Upgrades.Find(ID);
}

const FResourcesInfo* GameStatics::GetResourceByID(int32 ID)
{
	return ResourcesInfo.Find(ID);
}


void GameStatics::Initialize(const UDataTable& UnitInfoInit, const UDataTable& BuildingInfo, const UDataTable& UpgradeInfoInit, const UDataTable& ResourcesInfoInit)
{
	LoadUnits(UnitInfoInit);
	LoadBuildings(BuildingInfo);
	LoadUpgrades(UpgradeInfoInit);
	LoadResources(ResourcesInfoInit);
}

FUnitInfo* GameStatics::GetUnitReferenceByID(int32 ID)
{
	return Units.Find(ID);
}

FBuildingInfo* GameStatics::GetBuildingReferenceByID(int32 ID)
{
	return Buildings.Find(ID);
}
