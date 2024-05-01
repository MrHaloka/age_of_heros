#include "GamePlay/PlayerSpectatorPawn.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/HologramPoolingComponent.h"
#include "Components/PlayerResourceManagerComponent.h"
#include "Components/SphereComponent.h"
#include "Enums/PlayerActivity.h"
#include "Enums/Teams.h"
#include "GameFramework/SpringArmComponent.h"
#include "GamePlay/GameStatics.h"
#include "Managers/ObjectsManager.h"
#include "Managers/PlayerInputManager.h"

APlayerSpectatorPawn::APlayerSpectatorPawn()
{
	PrimaryActorTick.bCanEverTick = false;
	bAddDefaultMovementBindings = false;
	
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Spring Arm"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->TargetArmLength = 2000;
	SpringArmComponent->bDoCollisionTest = false;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->bUsePawnControlRotation = false;

	GetCollisionComponent()->SetGenerateOverlapEvents(false);
	GetCollisionComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCollisionComponent()->SetCollisionProfileName(TEXT("No Collision"));
	GetCollisionComponent()->SetSimulatePhysics(false);
	
	PlayerActivity = EPlayerActivity::Other;
}

void APlayerSpectatorPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputManager = NewObject<UPlayerInputManager>(this, PlayerInputManagerClass, NAME_None, RF_NoFlags, PlayerInputManagerClass->GetDefaultObject(), true);
	PlayerInputManager->Initialize(this,PlayerInputComponent);
}

void APlayerSpectatorPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayerSpectatorPawn::BeginPlay()
{
	check(PlayerInputManager != nullptr)
	Super::BeginPlay();
	
	HologramPoolingComponent = NewObject<UHologramPoolingComponent>(this, UHologramPoolingComponent::StaticClass(), TEXT("Holograms pooling"));
	HologramPoolingComponent->Initialize(GameStatics::GetObjectManager(), GetWorld(), NotAcceptableMaterial);
	HologramPoolingComponent->RegisterComponent();
	AddInstanceComponent(HologramPoolingComponent);

	PlayerResourceManagerComponent = NewObject<UPlayerResourceManagerComponent>(this, UPlayerResourceManagerComponent::StaticClass(), TEXT("Resource Manger"));
	PlayerResourceManagerComponent->RegisterComponent();
	AddInstanceComponent(PlayerResourceManagerComponent);
	
	PlayerInputManager->OnPlayerBegunPlay();
}

void APlayerSpectatorPawn::Destroyed()
{
	Super::Destroyed();
}

void APlayerSpectatorPawn::ChangePlayerActivity(EPlayerActivity NewPlayerActivity)
{
	PlayerActivity = NewPlayerActivity;
}

EPlayerActivity APlayerSpectatorPawn::GetPlayerActivity()
{
	return PlayerActivity;
}

USpringArmComponent* APlayerSpectatorPawn::GetSpringArmComponent()
{
	return SpringArmComponent;
}

ETeams APlayerSpectatorPawn::GetPlayerTeam()
{
	return ETeams::Blue;
}

ARTSHUD* APlayerSpectatorPawn::GetRTSHUD()
{
	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	return Cast<ARTSHUD>(PlayerController->GetHUD());
}
