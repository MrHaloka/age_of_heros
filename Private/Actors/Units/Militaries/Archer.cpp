#include "Actors/Units/Militaries/Archer.h"

#include "Components/AI/SightComponent.h"
#include "Components/Combat/RangeAttackComponent.h"

AArcher::AArcher() : AMoveableUnit()
{
	SightComponent = CreateDefaultSubobject<USightComponent>(TEXT("Sight Component"));
	RangeAttackComponent = CreateDefaultSubobject<URangeAttackComponent>(TEXT("Range Attack Component"));
}

void AArcher::BeginPlay()
{
	Super::BeginPlay();
}
