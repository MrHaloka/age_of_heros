#include "Components/PlayerResourceManagerComponent.h"

#include "GamePlay/PlayerSpectatorPawn.h"
#include "UI/RTSHUD.h"

UPlayerResourceManagerComponent::UPlayerResourceManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	Resources.Gold = 200;
	Resources.Stone = 200;
	Resources.Food = 200;
	Resources.Wood = 200;
}

void UPlayerResourceManagerComponent::AddNewResources(FResources NewResource)
{
	Resources += NewResource;
	CallResourcePanelForUpdate();
}

void UPlayerResourceManagerComponent::RemoveResources(FResources RemoveResources)
{
	Resources -= RemoveResources;
}

FResources UPlayerResourceManagerComponent::GetResources() const
{
	return Resources;
}

void UPlayerResourceManagerComponent::CallResourcePanelForUpdate()
{
	RTSHUD->UpdateResources(Resources);
}

bool UPlayerResourceManagerComponent::Pay(const FResources& Price)
{
	if (Resources >= Price)
	{
		Resources -= Price;
		CallResourcePanelForUpdate();
		return true;
	}
	return false;
}

void UPlayerResourceManagerComponent::Refund(const FResources& RefundResources)
{
	Resources += RefundResources;
	CallResourcePanelForUpdate();
}

bool UPlayerResourceManagerComponent::CanPay(const FResources& Price)
{
	return Resources >= Price;
}

void UPlayerResourceManagerComponent::AddNewResources(const FSingleResource& Resource)
{
	Resources += Resource;
	CallResourcePanelForUpdate();
}

void UPlayerResourceManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	RTSHUD = CastChecked<APlayerSpectatorPawn>(GetOwner())->GetRTSHUD();
	CallResourcePanelForUpdate();
}
