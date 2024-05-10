#include "Components/CursorSelectionComponent.h"

#include "GamePlay/GameStatics.h"
#include "GamePlay/PlayerSpectatorPawn.h"
#include "GamePlay/RTSGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/ObjectsManager.h"

UCursorSelectionComponent::UCursorSelectionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCursorSelectionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCursorSelectionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

FVector2d UCursorSelectionComponent::PlayerClickedNewLocation()
{
	const APlayerSpectatorPawn* Player = StaticCast<APlayerSpectatorPawn*>(GetOwner());
	FVector Location, Direction;
	CastChecked<APlayerController>(Player->GetController())->DeprojectMousePositionToWorld(Location, Direction);
	double Distance = Location.Z / Direction.Z;
	FHitResult Hit;
	FVector End = Location - Direction * Distance;
	FCollisionQueryParams QueryParams = FCollisionQueryParams(SCENE_QUERY_STAT(CursorTracer), false, Player->GetOwner());
	if (Player->GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECC_Visibility, QueryParams))
	{
		return FVector2d(Hit.Location).ClampAxes(0, GameStatics::GetRTSGameMode()->GetMapSize());
	}
	return FVector2d(End);
}

void UCursorSelectionComponent::StoreTheWorldLocation()
{
	LastStoredLocation = PlayerClickedNewLocation();
}

FVector2d UCursorSelectionComponent::GetLastStoredLocation()
{
	return LastStoredLocation;
}
