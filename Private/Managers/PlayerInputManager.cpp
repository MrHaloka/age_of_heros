#include "Managers/PlayerInputManager.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Components/ScreenScrollComponent.h"
#include "Components/UnitControllerComponent.h"
#include "Components/CursorSelectionComponent.h"
#include "Enums/PlayerActivity.h"
#include "GameFramework/SpringArmComponent.h"
#include "GamePlay/PlayerSpectatorPawn.h"
#include "GamePlay/RTSGameMode.h"
#include "GamePlay/RTSPlayerController.h"
#include "Kismet/GameplayStatics.h"

UPlayerInputManager::UPlayerInputManager()
{
}

void UPlayerInputManager::OnPlayerBegunPlay()
{
	SpringArmComponent = Player->GetSpringArmComponent();
	if (ARTSPlayerController* PlayerController = CastChecked<ARTSPlayerController>(Player->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(MainInput, 0);
		}
	}
}

void UPlayerInputManager::Initialize(APlayerSpectatorPawn* InitPlayer, UInputComponent* InputComponent)
{
	Player = InitPlayer;
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &UPlayerInputManager::Zoom);
		EnhancedInputComponent->BindAction(SelectAction, ETriggerEvent::Started, this, &UPlayerInputManager::Select);
		EnhancedInputComponent->BindAction(SelectAction, ETriggerEvent::Triggered, this, &UPlayerInputManager::SelectStarted);
		EnhancedInputComponent->BindAction(SelectAction, ETriggerEvent::Completed, this, &UPlayerInputManager::SelectEnded);
		EnhancedInputComponent->BindAction(UnselectAction, ETriggerEvent::Started, this, &UPlayerInputManager::UnSelect);
	}
	
	ScrollComponent = NewObject<UScreenScrollComponent>(Player, UScreenScrollComponent::StaticClass(), TEXT("Screen Scroll"));
	ScrollComponent->RegisterComponent();
	Player->AddInstanceComponent(ScrollComponent);

	CursorSelectionComponent = NewObject<UCursorSelectionComponent>(Player, UCursorSelectionComponent::StaticClass(), TEXT("Cursor Selection"));
	ScrollComponent->RegisterComponent();
	Player->AddInstanceComponent(CursorSelectionComponent);
	
	UnitControllerComponent = NewObject<UUnitControllerComponent>(Player, UUnitControllerComponent::StaticClass(), TEXT("Unit controller"));
	UnitControllerComponent->RegisterComponent();
	Player->AddInstanceComponent(UnitControllerComponent);
}

UPlayerInputManager::~UPlayerInputManager()
{
}

void UPlayerInputManager::Zoom(const FInputActionValue& InputActionValue)
{
	if (InputActionValue.Get<float>() > 0 && SpringArmComponent->TargetArmLength > MinZoomIn)
	{
		SpringArmComponent->TargetArmLength += -200;
	}
	if (InputActionValue.Get<float>() < 0 && SpringArmComponent->TargetArmLength < MaxZoomOut)
	{
		SpringArmComponent->TargetArmLength += 200;
	}
}

void UPlayerInputManager::Select()
{
	CursorSelectionComponent->StoreTheWorldLocation();
}

void UPlayerInputManager::SelectEnded()
{
	const FVector2d EndLocation = CursorSelectionComponent->PlayerClickedNewLocation();
	if (EndLocation.Equals(CursorSelectionComponent->GetLastStoredLocation()))
	{
		if (Player->GetPlayerActivity() == EPlayerActivity::Building)
		{
			Player->GetRTSHUD()->GetVillagerPanel()->PlaceBuilding(EndLocation);
			return;
		}
		UnitControllerComponent->NewLocationSelected(EndLocation);
	}
	else
	{
		UnitControllerComponent->SelectionBox(CursorSelectionComponent->GetLastStoredLocation(), EndLocation);
	}
	Player->GetRTSHUD()->EndSelection();
}

void UPlayerInputManager::SelectStarted()
{
	UGameViewportClient* GameViewport = Player->GetWorld()->GetGameViewport();
	check(GameViewport);
	if (GameViewport->Viewport->HasFocus())
	{
		FVector2D MousePosition;
		if (GameViewport->GetMousePosition(MousePosition))
		{
			FVector2D ScreenPosition = FVector2D(
		   MousePosition.X / GameViewport->Viewport->GetSizeXY().X,
		   MousePosition.Y / GameViewport->Viewport->GetSizeXY().Y
			);
			Player->GetRTSHUD()->UpdateSelectionBox(ScreenPosition);
		}
	}
}

void UPlayerInputManager::UnSelect()
{
	Player->GetRTSHUD()->RemoveTheActivePanel();
	UnitControllerComponent->UnselectUnits();
	if (Player->GetPlayerActivity() == EPlayerActivity::Building)
	{
		Player->GetRTSHUD()->GetVillagerPanel()->ResetBuilding();
	}
}
