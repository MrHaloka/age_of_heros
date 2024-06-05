#include "Actors/Units/Militaries/Scout.h"

#include "Components/AI/SightComponent.h"
#include "Components/Combat/MeleeAttackComponent.h"


AScout::AScout() : AMoveableUnit()
{
	MeleeAttackComponent = CreateDefaultSubobject<UMeleeAttackComponent>(TEXT("Melee Attack Component"));
	SightComponent = CreateDefaultSubobject<USightComponent>(TEXT("Sight Component"));
}

void AScout::BeginPlay()
{
	Super::BeginPlay();
}