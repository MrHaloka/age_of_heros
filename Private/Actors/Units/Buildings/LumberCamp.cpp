#include "Actors/Units/Buildings/LumberCamp.h"
#include "Components/DropoffComponent.h"

ALumberCamp::ALumberCamp() : ABaseBuilding()
{
}

void ALumberCamp::BeginPlay()
{
	Super::BeginPlay();
	WoodDropoffComponent = NewObject<UDropoffComponent>(this, UDropoffComponent::StaticClass(), TEXT("Wood Dropoff"));
	WoodDropoffComponent->SetDropoffType(EResource::Wood);
	WoodDropoffComponent->RegisterComponent();
	AddInstanceComponent(WoodDropoffComponent);
}