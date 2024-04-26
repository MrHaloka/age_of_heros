#include "Components/ScreenScrollComponent.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "GameFramework/PawnMovementComponent.h"

UScreenScrollComponent::UScreenScrollComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UScreenScrollComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerPawn = CastChecked<APlayerSpectatorPawn>(GetOwner());
}

void UScreenScrollComponent::BeginDestroy()
{
	UE_LOG(LogTemp, Warning, TEXT("component got destroy"));
	Super::BeginDestroy();
}

void UScreenScrollComponent::Scroll()
{
	FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(OwnerPawn->GetWorld());
	FVector2D ViewSize = UWidgetLayoutLibrary::GetViewportSize(OwnerPawn->GetWorld());
	MousePosition *= UWidgetLayoutLibrary::GetViewportScale(OwnerPawn->GetWorld());
	MousePosition.X /= ViewSize.X;
	MousePosition.Y /= ViewSize.Y;
	if (MousePosition.X > 0.95)
	{
		OwnerPawn->MoveRight(OwnerPawn->GetMovementComponent()->GetMaxSpeed());
	}
	else if (MousePosition.X < 0.05)
	{
		OwnerPawn->MoveRight(-OwnerPawn->GetMovementComponent()->GetMaxSpeed());
	}
	if (MousePosition.Y > 0.95)
	{
		OwnerPawn->MoveForward(-OwnerPawn->GetMovementComponent()->GetMaxSpeed());
	}
	else if (MousePosition.Y < 0.05)
	{
		OwnerPawn->MoveForward(OwnerPawn->GetMovementComponent()->GetMaxSpeed());
	}
}


void UScreenScrollComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	Scroll();
}

void UScreenScrollComponent::SetSensitivity(uint8 NewXSensitivityPercent, uint8 NewYSensitivityPercent)
{
	XSensitivityPercent = NewXSensitivityPercent;
	YSensitivityPercent = NewYSensitivityPercent;
}

