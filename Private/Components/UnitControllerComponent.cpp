#include "Components/UnitControllerComponent.h"

#include "ChainHandlers/CursorInputHandler/BuildingHandler.h"
#include "ChainHandlers/CursorInputHandler/ConstructionSiteHandler.h"
#include "ChainHandlers/CursorInputHandler/EmptyLocationHandler.h"
#include "ChainHandlers/CursorInputHandler/EnemyUnitHandler.h"
#include "ChainHandlers/CursorInputHandler/FriendlyMoveableHandler.h"
#include "ChainHandlers/CursorInputHandler/ResourceHandler.h"
#include "GamePlay/GameStatics.h"
#include "GamePlay/PlayerSpectatorPawn.h"
#include "Managers/ObjectsManager.h"
#include "Actors/Units/Villager.h"

UUnitControllerComponent::UUnitControllerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UUnitControllerComponent::BeginPlay()
{
	Super::BeginPlay();
	APlayerSpectatorPawn* OwnerPlayer = CastChecked<APlayerSpectatorPawn>(GetOwner());
	Player = OwnerPlayer;
	ObjectsManager = GameStatics::GetObjectManager();
	CreateHandlers();
}

void UUnitControllerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UUnitControllerComponent::UnselectUnits()
{
	SelectedUnits.Empty();
}

/**
 * This method creates a chain of handlers to process the location where the player clicked.
 * If a handler has a valid response for the input, it will handle it; otherwise, it will pass
 * the input forward in the chain for the next handler to process.
 */

void UUnitControllerComponent::CreateHandlers()
{
	TUniquePtr<FBuildingHandler> BuildingHandler = MakeUnique<FBuildingHandler>(SelectedUnits, *Player);
	TUniquePtr<FConstructionSiteHandler> ConstructionSiteHandler = MakeUnique<FConstructionSiteHandler>(SelectedUnits, MoveTemp(BuildingHandler));
	TUniquePtr<FFriendlyMoveableHandler> FriendlyMoveableHandler = MakeUnique<FFriendlyMoveableHandler>(SelectedUnits, Player, MoveTemp(ConstructionSiteHandler));
	TUniquePtr<FEnemyUnitHandler> EnemyUnitHandler = MakeUnique<FEnemyUnitHandler>(SelectedUnits, MoveTemp(FriendlyMoveableHandler), Player->GetPlayerTeam());
	TUniquePtr<FResourceHandler> ResourceHandler = MakeUnique<FResourceHandler>(SelectedUnits, MoveTemp(EnemyUnitHandler));
	InputChainHandler = MakeUnique<FEmptyLocationHandler>(SelectedUnits, MoveTemp(ResourceHandler));
}

void UUnitControllerComponent::NewLocationSelected(const FVector2d& Location)
{
	check(ObjectsManager != nullptr)
	AActor* NewSelectedUnit = ObjectsManager->GetActorInLocation(Location);
	InputChainHandler->Execute(NewSelectedUnit, Location);
}