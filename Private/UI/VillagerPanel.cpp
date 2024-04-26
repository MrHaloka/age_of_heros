#include "UI/VillagerPanel.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/HologramPoolingComponent.h"
#include "Enums/PlayerActivity.h"
#include "Enums/Teams.h"
#include "GamePlay/PlayerSpectatorPawn.h"
#include "Helpers/GridHelper.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/ObjectsManager.h"

void UVillagerPanel::NativeConstruct()
{
	Super::NativeConstruct();
	FScriptDelegate ShowHouse;
	WidgetTree->FindWidget<UButton>(TEXT("Build"))->OnClicked.AddDynamic(this, &UVillagerPanel::ShowBuildPanel);
	WidgetTree->FindWidget<UButton>(TEXT("House"))->OnClicked.AddDynamic(this, &UVillagerPanel::ShowHouse);
	WidgetTree->FindWidget<UButton>(TEXT("LumberCamp"))->OnClicked.AddDynamic(this, &UVillagerPanel::ShowLumberCamp);
	WidgetTree->FindWidget<UButton>(TEXT("MiningCamp"))->OnClicked.AddDynamic(this, &UVillagerPanel::ShowMiningCamp);
	MainMenu = WidgetTree->FindWidget<UBorder>(TEXT("Menu_Panel"));
	BuildingMenu = WidgetTree->FindWidget<UBorder>(TEXT("Build_Panel"));
	BuildingMenu->SetVisibility(ESlateVisibility::Hidden);
}

void UVillagerPanel::SetVisibility(ESlateVisibility InVisibility)
{
	Super::SetVisibility(InVisibility);
	if (InVisibility == ESlateVisibility::Hidden)
	{
		ResetToDefault();
	}
}

void UVillagerPanel::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (TickFrequency == EWidgetTickFrequency::Never)
	{
		return;
	}
	ChangeBuildingLocationBaseOnMouse();
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UVillagerPanel::PlaceBuilding(const FVector2d& Location)
{
	GetOwningPlayerPawn()->GetComponentByClass<UHologramPoolingComponent>()->StartBuilding(Location, ETeams::Blue);
	ResetBuilding();
}

void UVillagerPanel::ResetBuilding()
{
	TickFrequency = EWidgetTickFrequency::Never;
	APlayerSpectatorPawn* PlayerSpectatorPawn = Cast<APlayerSpectatorPawn>(GetOwningPlayerPawn());
	PlayerSpectatorPawn->ChangePlayerActivity(EPlayerActivity::Other);
	GetOwningPlayerPawn()->GetComponentByClass<UHologramPoolingComponent>()->ResetActiveHologram();
}

void UVillagerPanel::ShowBuildPanel()
{
	BuildingMenu->SetVisibility(ESlateVisibility::Visible);
}

void UVillagerPanel::ShowBuilding(uint32 BuildingId)
{
	GetOwningPlayerPawn()->GetComponentByClass<UHologramPoolingComponent>()->SetActiveId(BuildingId);
	ChangeBuildingLocationBaseOnMouse();
	GetOwningPlayerPawn()->GetComponentByClass<UHologramPoolingComponent>()->ShowBuilding();
	APlayerSpectatorPawn* PlayerSpectatorPawn = Cast<APlayerSpectatorPawn>(GetOwningPlayerPawn());
	PlayerSpectatorPawn->ChangePlayerActivity(EPlayerActivity::Building);
	TickFrequency = EWidgetTickFrequency::Auto;
}

void UVillagerPanel::ShowLumberCamp()
{
	ShowBuilding(3);
}

void UVillagerPanel::ShowHouse()
{
	ShowBuilding(2);
}

void UVillagerPanel::ShowMiningCamp()
{
	ShowBuilding(4);
}

void UVillagerPanel::ResetToDefault()
{
	if (BuildingMenu != nullptr)
	{
		BuildingMenu->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UVillagerPanel::ChangeBuildingLocationBaseOnMouse()
{
	FVector Location, Direction;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->DeprojectMousePositionToWorld(Location, Direction);
	FHitResult Hit;
	FVector End = Location + Direction * 10000;
	FCollisionQueryParams QueryParams = FCollisionQueryParams(SCENE_QUERY_STAT(WeaponTrace), false, GetOwningPlayer());
	if (GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECC_Visibility, QueryParams))
	{
		if(!GetOwningPlayerPawn()->GetComponentByClass<UHologramPoolingComponent>()->CanPlaceOn(FVector2d(Hit.Location)))
		{
			GetOwningPlayerPawn()->GetComponentByClass<UHologramPoolingComponent>()->SetNotAcceptedMaterial();
		}
		else
		{
			GetOwningPlayerPawn()->GetComponentByClass<UHologramPoolingComponent>()->SetNormalMaterial();
		}
		GetOwningPlayerPawn()->GetComponentByClass<UHologramPoolingComponent>()->MoveTo(FVector(GridHelper::GetGridFromVector(Hit.Location) * 100, 0));
	}
}
