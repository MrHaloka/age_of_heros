#include "Components/DropoffComponent.h"

#include "Components/PlayerResourceManagerComponent.h"
#include "DataStructures/SingleResource.h"
#include "Enums/Resource.h"
#include "GamePlay/PlayerSpectatorPawn.h"
#include "GamePlay/RTSGameMode.h"
#include "Helpers/GridHelper.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/DropoffsManager.h"

UDropoffComponent::	UDropoffComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

UDropoffComponent::~UDropoffComponent()
{
}

void UDropoffComponent::SetDropoffType(EResource Resource)
{
	ResourceType = Resource;
}

EResource UDropoffComponent::GetResourceType() const
{
	return ResourceType;
}

void UDropoffComponent::SetID(uint32 NewID)
{
	ID = NewID;
}

uint32 UDropoffComponent::GetID() const
{
	return ID;
}

FVector2d UDropoffComponent::GetGridLocation()
{
	return FVector2d(FMath::Floor(GetOwner()->GetActorLocation().X / 100), FMath::Floor(GetOwner()->GetActorLocation().Y / 100));;
}

ABaseUnit* UDropoffComponent::GetBaseUnitOwner()
{
	check(Cast<ABaseUnit>(GetOwner()))
	return Cast<ABaseUnit>(GetOwner());
}

void UDropoffComponent::DroppedResources(FSingleResource Resource)
{
	APlayerSpectatorPawn* PlayerSpectatorPawn = CastChecked<APlayerSpectatorPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	PlayerSpectatorPawn->GetComponentByClass<UPlayerResourceManagerComponent>()->AddNewResources(Resource);
}

void UDropoffComponent::BeginPlay()
{
	Super::BeginPlay();
	if (ARTSGameMode* GameMode = Cast<ARTSGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		GameMode->GetDropoffManager()->AddNewDropoff(this);
	}
}

void UDropoffComponent::BeginDestroy()
{
	if (HasBegunPlay())
	{
		if (ARTSGameMode* GameMode = Cast<ARTSGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			GameMode->GetDropoffManager()->RemoveDropoff(this);
		}
	}
	Super::BeginDestroy();
}


void UDropoffComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

