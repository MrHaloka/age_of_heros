#pragma once

#include "CoreMinimal.h"
#include "UI/BuildingsPanelWidget.h"
#include "TownCenterPanel.generated.h"

class UProgressBar;
class UButton;
/**
 * 
 */
UCLASS()
class AOE2_API UTownCenterPanel : public UBuildingsPanelWidget
{
	GENERATED_BODY()

public:
	UTownCenterPanel(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;
	UFUNCTION()
	void CreateVillager();
	UFUNCTION()
	void GetLoom();
	UFUNCTION()
	void AgeUp();
	UFUNCTION()
	void LoomFinished();
	UFUNCTION()
	void LoomCanceled();
protected:
	UPROPERTY()
	UProgressBar* ProgressBar;
	float CurrentProgressBarSecond = 0;
	FTimerHandle ProgressTimerHandle;
	void ClearProgressBar();
	UFUNCTION()
	void RunProgressBar(const uint32& Seconds);
	void StartProgressBar(uint32 Seconds);

};
