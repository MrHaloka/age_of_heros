#include "Actors/Units/Buildings/Tower.h"

#include "Components/AI/SightComponent.h"
#include "Components/Combat/RangeAttackComponent.h"

ATower::ATower() : ABaseBuilding()
{
	SightComponent = CreateDefaultSubobject<USightComponent>(TEXT("Sight Component"));
	RangeAttackComponent = CreateDefaultSubobject<URangeAttackComponent>(TEXT("Range Attack Component"));
}

void ATower::BeginPlay()
{
	Super::BeginPlay();
}
