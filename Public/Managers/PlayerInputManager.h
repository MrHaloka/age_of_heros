#pragma once

#include "CoreMinimal.h"
#include "PlayerInputManager.generated.h"

class UUnitControllerComponent;
class UCursorSelectionComponent;
class UScreenScrollComponent;
class USpringArmComponent;
class APlayerSpectatorPawn;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class AOE2_API UPlayerInputManager : public UObject 
{
	GENERATED_BODY()
public:
	UPlayerInputManager();
	virtual ~UPlayerInputManager() override;
	void Initialize(APlayerSpectatorPawn* Player, UInputComponent* InputComponent);
	void Zoom(const FInputActionValue& InputActionValue);
	void Select();
	void UnSelect();
	void OnPlayerBegunPlay();

protected:
	UPROPERTY(EditDefaultsOnly, Category=Input)
	UInputAction* ScrollAction;
	UPROPERTY(EditDefaultsOnly, Category=Input)
	UInputAction* ZoomAction;
	UPROPERTY(EditDefaultsOnly, Category=Input)
	UInputAction* SelectAction;
	UPROPERTY(EditDefaultsOnly, Category=Input)
	UInputAction* UnselectAction;
	UPROPERTY(EditDefaultsOnly, Category=Input)
	UInputMappingContext* MainInput;
	UPROPERTY(EditDefaultsOnly, Category=Input)
	float MaxZoomOut = 2000;
	UPROPERTY(EditDefaultsOnly, Category=Input)
	float MinZoomIn = 1000;
	const FVector2d InvalidLocation;
	USpringArmComponent* SpringArmComponent;
	APlayerSpectatorPawn* Player;
	UCursorSelectionComponent* CursorSelectionComponent;
	UUnitControllerComponent* UnitControllerComponent;
	UScreenScrollComponent* ScrollComponent;
};
