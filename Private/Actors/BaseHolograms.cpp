
#include "Actors/BaseHolograms.h"

ABaseHolograms::ABaseHolograms()
{
	PrimaryActorTick.bCanEverTick = false;

	MainMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hologram Mesh"));
	MainMesh->SetMobility(EComponentMobility::Movable);
	MainMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SetRootComponent(MainMesh);
	
	AActor::SetActorHiddenInGame(true);
}

void ABaseHolograms::Show()
{
	SetActorHiddenInGame(false);
}

void ABaseHolograms::ResetHologram()
{
	MainMesh->SetMaterial(0, DefaultMaterial);
	SetActorLocation(FVector::Zero());
	SetActorHiddenInGame(true);
}

void ABaseHolograms::SetNewMaterial(UMaterial* NewMaterial)
{
	MainMesh->SetMaterial(0, NewMaterial);
}

void ABaseHolograms::SetNormalMaterial()
{
	MainMesh->SetMaterial(0, DefaultMaterial);
}

void ABaseHolograms::BeginPlay()
{
	Super::BeginPlay();
	DefaultMaterial = MainMesh->GetMaterial(0)->GetMaterial();
}

