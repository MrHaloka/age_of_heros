#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enums/UnitState.h"
#include "BaseUnit.generated.h"
class ARTSHUD;
enum class ETeams;
enum EUnitState : int;
struct FResources;

UCLASS()
class AOE2_API ABaseUnit : public AActor
{
	GENERATED_BODY()

public:
	ABaseUnit();

protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly)
	float HP;
	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EUnitState> UnitState;
	uint32 ID;
	ETeams Team;
public:
	virtual void Tick(float DeltaTime) override;
	EUnitState GetUnitState() const;
	virtual void SetUnitState(TEnumAsByte<EUnitState> NewUnitState);
	float GetHP() const;
	void SetHP(const float& HP);
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,AActor* DamageCauser) override;
	virtual float GetMaxHP();
	virtual FResources GetPrice();
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual FVector2d GetSize();
	FVector2d GetActorLocation2d() const;
	virtual int32 GetCollisionRadius() const;
	void SetID(const uint32& NewID);
	uint32 GetID() const;
	virtual void ShowUnitPanel(ARTSHUD* TargetHUD);
	int32 GetClassID();
	void SetTeam(ETeams NewTeam);
	ETeams GetTeam();
protected:
	DECLARE_EVENT_TwoParams(ABaseUnit, FOnPrepareUnitStateChangeEvent, TEnumAsByte<EUnitState>, TEnumAsByte<EUnitState>);
	FOnPrepareUnitStateChangeEvent OnPrepareUnitStateChangeEvent;
	UFUNCTION()
	virtual void OnPrepareUnitStateChanged(TEnumAsByte<EUnitState> NewState, TEnumAsByte<EUnitState> OldState);
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "StateChanged"))
	void UnitStateChanged(EUnitState NewState);
public:
	FOnPrepareUnitStateChangeEvent& GetPrepareUnitStateEventHandler();
};
