#include "Components/AI/SightComponent.h"

#include "GamePlay/GameStatics.h"
#include "GamePlay/RTSGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/ObjectsManager.h"
#include "Managers/PerceptionManager.h"

USightComponent::USightComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USightComponent::BeginPlay()
{
	Super::BeginPlay();
	GameStatics::GetObjectManager()->GetPerceptionManager()->RegisterUnitSightPerception(StaticCast<ABaseUnit*>(GetOwner()));
}

void USightComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (ARTSGameMode* GameMode = Cast<ARTSGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		GameMode->GetObjectManager()->GetPerceptionManager()->RemoveUnitSightPerception(StaticCast<ABaseUnit*>(GetOwner()));
	}
	Super::EndPlay(EndPlayReason);
}

FVector2d USightComponent::GetPerceptionCenter()
{
	return StaticCast<ABaseUnit*>(GetOwner())->GetActorLocation2d();
}

uint32 USightComponent::GetPerceptionRadius()
{
	return PerceptionRadius;
}

void USightComponent::Sensed(const ABaseUnit* Unit)
{
	OnSenseUnitEvent.Broadcast(Unit);
}

uint32 USightComponent::GetPerceptionRadiusSquare()
{
	return PerceptionRadius*PerceptionRadius;
}

ABaseUnit* USightComponent::GetFirstEnemyInPerception()
{
	if (ARTSGameMode* GameMode = Cast<ARTSGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		return GameMode->GetObjectManager()->GetPerceptionManager()->GetFirstEnemyInPerception(StaticCast<ABaseUnit*>(GetOwner()));
	}
	return nullptr;
}