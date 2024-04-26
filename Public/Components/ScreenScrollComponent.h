#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GamePlay/PlayerSpectatorPawn.h"
#include "ScreenScrollComponent.generated.h"
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AOE2_API UScreenScrollComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UScreenScrollComponent();
protected:
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
	void Scroll();
	uint8 XSensitivityPercent;
	uint8 YSensitivityPercent;
	UPROPERTY()
	APlayerSpectatorPawn* OwnerPawn;
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void SetSensitivity(uint8 NewXSensitivityPercent, uint8 NewYSensitivityPercent);
};
