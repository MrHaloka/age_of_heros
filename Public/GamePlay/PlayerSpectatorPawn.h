#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpectatorPawn.h"
#include "UI/RTSHUD.h"
#include "PlayerSpectatorPawn.generated.h"

class UPlayerResourceManagerComponent;
enum class EPlayerActivity;
enum class ETeams;
class UCursorSelectionComponent;
class UCameraComponent;
class UInputComponent;
class USpringArmComponent;
class UPlayerInputManager;
class FSelectUnitManager;
class UHologramPoolingComponent;
UCLASS()
class AOE2_API APlayerSpectatorPawn : public ASpectatorPawn
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterial* NotAcceptableMaterial;
	UPROPERTY(EditAnywhere, Category =  Input)
	TSubclassOf<UPlayerInputManager> PlayerInputManagerClass;
	UPROPERTY()
	UPlayerInputManager* PlayerInputManager;
	UPROPERTY(EditAnywhere, Category = Camera)
	UCameraComponent* CameraComponent;
	UPROPERTY(EditAnywhere, Category =  Camera)
	USpringArmComponent* SpringArmComponent;
	EPlayerActivity PlayerActivity;
	UPROPERTY()
	UHologramPoolingComponent* HologramPoolingComponent;
	UPROPERTY()
	UPlayerResourceManagerComponent* PlayerResourceManagerComponent;
public:
	APlayerSpectatorPawn();
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	void ChangePlayerActivity(EPlayerActivity NewPlayerActivity);
	EPlayerActivity GetPlayerActivity();
	USpringArmComponent* GetSpringArmComponent();
	ETeams GetPlayerTeam();
	ARTSHUD* GetRTSHUD();
};

