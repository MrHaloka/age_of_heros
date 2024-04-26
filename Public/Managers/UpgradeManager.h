#pragma once

struct FUpgrades;
class ABaseUnit;
enum EUpgradeType : int;
/**
 * 
 */
typedef TFunctionRef<void (TSubclassOf<ABaseUnit>, int32)> FunctionType;
class AOE2_API FUpgradeManager
{
public:
	FUpgradeManager();
	~FUpgradeManager();
	FTimerHandle Timer;
	void Register(EUpgradeType NewType, FunctionType Function);
	void Call(FUpgrades* Upgrade);
	void Call(int32 UpgradeID);
	void CallPrepare(uint32 UpgradeID, const FTimerDelegate& OnFinish);
	void CallPrepare(uint32 UpgradeID, FUpgrades* Upgrade, const FTimerDelegate& OnFinish);
	void Cancel(int ID);
	TMap<EUpgradeType, FunctionType> UpgradesFunction;
	static void AddMaxHP(TSubclassOf<ABaseUnit> TargetClass, int32 HP);
protected:
	bool CanPayThePrice(FUpgrades* Upgrade);
	void Refund(FUpgrades* Upgrade);
	TMap<uint32, FTimerHandle> UpgradesTimeHandler;
};
