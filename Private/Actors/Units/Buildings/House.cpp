#include "Actors/Units/Buildings/House.h"

#include "Components/PlayerResourceManagerComponent.h"
#include "GamePlay/PlayerSpectatorPawn.h"
#include "Kismet/GameplayStatics.h"

void AHouse::BeginPlay()
{
	Super::BeginPlay();
	// APlayerSpectatorPawn* PlayerSpectatorPawn = CastChecked<APlayerSpectatorPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	// PlayerSpectatorPawn->GetComponentByClass<UPlayerResourceManagerComponent>()->AddNewResources(FResources(EResource::Population, 5));
}

void AHouse::BeginDestroy()
{
	// APlayerSpectatorPawn* PlayerSpectatorPawn = CastChecked<APlayerSpectatorPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	// PlayerSpectatorPawn->GetComponentByClass<UPlayerResourceManagerComponent>()->RemoveResources(FResources(EResource::Population, 5));
	Super::BeginDestroy();
}
