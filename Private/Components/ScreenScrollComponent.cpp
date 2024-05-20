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
	check(GetOwner()->IsA(APlayerSpectatorPawn::StaticClass()))
	MovementSpeed = StaticCast<APlayerSpectatorPawn*>(GetOwner())->GetMovementComponent()->GetMaxSpeed();
}

void UScreenScrollComponent::BeginDestroy()
{
	Super::BeginDestroy();
}

void UScreenScrollComponent::Scroll()
{
	FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetOwner()->GetWorld());
	MousePosition *= UWidgetLayoutLibrary::GetViewportScale(GetOwner()->GetWorld());
	MousePosition /= UWidgetLayoutLibrary::GetViewportSize(GetOwner()->GetWorld());
	if (MousePosition.X > 0.95 || MousePosition.X < 0.05 || MousePosition.Y > 0.95 || MousePosition.Y < 0.05)
	{
		const FVector2d Direction(0.5 - MousePosition.Y  ,MousePosition.X - 0.5 );
		const FVector Movement(RotationMatrix.TransformVector(Direction.GetSafeNormal() * MovementSpeed), 0);
		StaticCast<APlayerSpectatorPawn*>(GetOwner())->AddMovementInput(Movement);
	}
}


void UScreenScrollComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	Scroll();
}