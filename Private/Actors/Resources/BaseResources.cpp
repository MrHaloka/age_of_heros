#include "Actors/Resources/BaseResources.h"
#include "DataStructures/ResourcesInfo.h"
#include "Enums/Teams.h"
#include "GamePlay/GameStatics.h"
#include "GamePlay/RTSGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/ObjectsManager.h"

ABaseResources::ABaseResources()
{
	PrimaryActorTick.bCanEverTick = false;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Resource Mesh"));
	StaticMesh->SetComponentTickEnabled(false);
	SetRootComponent(StaticMesh);
}

ETeams ABaseResources::GetTeam()
{
	return ETeams::White;
}

FVector2d ABaseResources::GetActorLocation2d() const
{
	return FVector2d(GetActorLocation());
}

int32 ABaseResources::GetResourceId() const
{
	return ResourceId;
}

void ABaseResources::SetID(uint32 NewID)
{
	ID = NewID;
}

uint32 ABaseResources::GetID() const
{
	return ID;
}

void ABaseResources::RemoveFromResource(float Amount)
{
	ResourceAvailable -= Amount;
	if (ResourceAvailable <= 0)
	{
		Destroy();
	}
}

void ABaseResources::BeginPlay()
{
	Super::BeginPlay();
	ResourceAvailable = GetResourceInfo()->MaxResource;
}

const FResourcesInfo* ABaseResources::GetResourceInfo() const
{
	return GameStatics::GetResourceByID(ResourceId);
}

void ABaseResources::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (ARTSGameMode* GameMode = Cast<ARTSGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		GameMode->GetObjectManager()->RemoveResourceUnsafe(ID, GetActorLocation2d());
	}
	Super::EndPlay(EndPlayReason);
}