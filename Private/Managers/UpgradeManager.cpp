#include "Managers/UpgradeManager.h"

#include "Components/PlayerResourceManagerComponent.h"
#include "DataStructures/BuildingInfo.h"
#include "DataStructures/UnitInfo.h"
#include "DataStructures/Upgrades.h"
#include "Enums/UpgradeType.h"
#include "GamePlay/GameStatics.h"
#include "GamePlay/PlayerSpectatorPawn.h"
#include "GamePlay/RTSGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Engine/Public/TimerManager.h"

FUpgradeManager::FUpgradeManager()
{
	Register(EUpgradeType::Max_HP, FUpgradeManager::AddMaxHP);
}

FUpgradeManager::~FUpgradeManager()
{
}

void FUpgradeManager::Register(EUpgradeType NewType, FunctionType Function)
{
	UpgradesFunction.Add(NewType, Function);
}

void FUpgradeManager::Call(FUpgrades* Upgrade)
{
	(*UpgradesFunction.Find(Upgrade->Type))(Upgrade->TargetUnit, Upgrade->Amount);
}

void FUpgradeManager::Call(int32 UpgradeID)
{
	Call(GameStatics::GetUpgradeByID(UpgradeID));
}

void FUpgradeManager::CallPrepare(uint32 UpgradeID, const FTimerDelegate& OnFinish)
{
	CallPrepare(UpgradeID, GameStatics::GetUpgradeByID(UpgradeID), OnFinish);
}

void FUpgradeManager::CallPrepare(uint32 UpgradeID, FUpgrades* Upgrade, const FTimerDelegate& OnFinish)
{
	if (CanPayThePrice(Upgrade))
	{
		FTimerHandle TimerHandle ;
		UpgradesTimeHandler.Add(UpgradeID, TimerHandle);
		GEngine->GetCurrentPlayWorld()->GetTimerManager().SetTimer(*UpgradesTimeHandler.Find(UpgradeID), OnFinish, Upgrade->TimeToComplete,false);
	}
}

void FUpgradeManager::Refund(FUpgrades* Upgrade)
{
	APlayerSpectatorPawn* PlayerSpectatorPawn = CastChecked<APlayerSpectatorPawn>(UGameplayStatics::GetPlayerPawn(GEngine->GetCurrentPlayWorld(), 0));
	PlayerSpectatorPawn->GetComponentByClass<UPlayerResourceManagerComponent>()->Refund(Upgrade->GetPrice());
}

void FUpgradeManager::Cancel(int UpgradeID)
{
	if (UpgradesTimeHandler.Contains(UpgradeID))
	{
		GEngine->GetCurrentPlayWorld()->GetTimerManager().ClearTimer( *UpgradesTimeHandler.Find(UpgradeID));
		(*UpgradesTimeHandler.Find(UpgradeID)).Invalidate();
		Refund(GameStatics::GetUpgradeByID(UpgradeID));
		UpgradesTimeHandler.Remove(UpgradeID);
	}
}

void FUpgradeManager::AddMaxHP(TSubclassOf<ABaseUnit> TargetClass, int32 HP)
{
	uint32 ID = GameStatics::GetClassTypeId(TargetClass->GetFName());
	if (TargetClass->IsChildOf<ABaseBuilding>())
	{
		GameStatics::GetBuildingReferenceByID(ID)->MaxHP = HP;
	}
	else
	{
		GameStatics::GetUnitReferenceByID(ID)->MaxHP = HP;
	}
}

bool FUpgradeManager::CanPayThePrice(FUpgrades* Upgrade)
{
	APlayerSpectatorPawn* PlayerSpectatorPawn = CastChecked<APlayerSpectatorPawn>(UGameplayStatics::GetPlayerPawn(GEngine->GetCurrentPlayWorld(), 0));
	return PlayerSpectatorPawn->GetComponentByClass<UPlayerResourceManagerComponent>()->Pay(Upgrade->GetPrice());
}
