#include "Actors/Units/Buildings/MiningCamp.h"

#include "Components/DropoffComponent.h"

void AMiningCamp::BeginPlay()
{
	Super::BeginPlay();
	GoldDropoffComponent = NewObject<UDropoffComponent>(this, UDropoffComponent::StaticClass(), TEXT("Gold Dropoff"));
    GoldDropoffComponent->SetDropoffType(EResource::Gold);
    GoldDropoffComponent->RegisterComponent();
    AddInstanceComponent(GoldDropoffComponent);
   
    StoneDropoffComponent = NewObject<UDropoffComponent>(this, UDropoffComponent::StaticClass(), TEXT("Stone Dropoff"));
    StoneDropoffComponent->SetDropoffType(EResource::Stone);
    StoneDropoffComponent->RegisterComponent();
    AddInstanceComponent(StoneDropoffComponent);
}
