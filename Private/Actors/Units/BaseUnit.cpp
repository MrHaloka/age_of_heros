#include "Actors/Units/BaseUnit.h"

#include "Components/BuilderComponent.h"
#include "DataStructures/UnitInfo.h"
#include "Enums/UnitState.h"
#include "GamePlay/GameStatics.h"
#include "GamePlay/RTSGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/ObjectsManager.h"
#include "DataStructures/Resources.h"
#include "UI/RTSHUD.h"

ABaseUnit::ABaseUnit()
{
	PrimaryActorTick.bCanEverTick = false;
	bAsyncPhysicsTickEnabled = false;
	UnitState = Idle;
	OnPrepareUnitStateChangeEvent.AddUFunction(this, "OnPrepareUnitStateChanged");
}

void ABaseUnit::BeginPlay()
{
	Super::BeginPlay();
	HP = GetMaxHP();
}

void ABaseUnit::OnPrepareUnitStateChanged(TEnumAsByte<EUnitState> NewState, TEnumAsByte<EUnitState> OldState)
{
}

ABaseUnit::FOnPrepareUnitStateChangeEvent& ABaseUnit::GetPrepareUnitStateEventHandler()
{
	return OnPrepareUnitStateChangeEvent;
}

FVector2d ABaseUnit::GetSize()
{
	return FVector2d::Zero();
}

void ABaseUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float ABaseUnit::GetHP() const
{
	return HP;
}

void ABaseUnit::SetHP(const float& NewHP)
{
	HP = NewHP;
}

float ABaseUnit::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                            AActor* DamageCauser)
{
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

float ABaseUnit::GetMaxHP()
{
	return GameStatics::GetUnitReferenceByID(GameStatics::GetClassTypeId(this->GetClass()->GetFName()))->MaxHP;
}

FResources ABaseUnit::GetPrice()
{
	return GameStatics::GetUnitReferenceByID(GameStatics::GetClassTypeId(this->GetClass()->GetFName()))->GetPrice();
}

void ABaseUnit::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (ARTSGameMode* GameMode = Cast<ARTSGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		GameMode->GetObjectManager()->RemoveUnitUnsafe(ID, GetActorLocation2d());
		UE_LOG(LogTemp, Warning, TEXT("Id %d deleted"), ID)
	}
	Super::EndPlay(EndPlayReason);
}

FVector2d ABaseUnit::GetActorLocation2d() const
{
	return FVector2d(GetActorLocation());
}

int32 ABaseUnit::GetCollisionRadius() const
{
	return GameStatics::GetUnitReferenceByID(GameStatics::GetClassTypeId(this->GetClass()->GetFName()))->CollisionRadius;
}

void ABaseUnit::SetID(const uint32& NewID)
{
	this->ID = NewID;
}

uint32 ABaseUnit::GetID() const
{
	return ID;
}

void ABaseUnit::ShowUnitPanel(ARTSHUD* TargetHUD)
{
	TargetHUD->RemoveTheActivePanel();
}

int32 ABaseUnit::GetClassID()
{
	return GameStatics::GetClassTypeId(this->GetClass()->GetFName());
}

void ABaseUnit::SetTeam(ETeams NewTeam)
{
	Team = NewTeam;
}

ETeams ABaseUnit::GetTeam()
{
	return Team;
}

EUnitState ABaseUnit::GetUnitState() const
{
	return UnitState;
}

void ABaseUnit::SetUnitState(TEnumAsByte<EUnitState> NewUnitState)
{
	OnPrepareUnitStateChangeEvent.Broadcast(NewUnitState, UnitState);
	UnitState = NewUnitState;
	UnitStateChanged(NewUnitState.GetValue());
}
