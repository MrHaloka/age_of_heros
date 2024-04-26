#include "Components/HologramPoolingComponent.h"

#include "Components/BuildingConstructComponent.h"
#include "Components/PlayerResourceManagerComponent.h"
#include "DataStructures/BuildingInfo.h"
#include "GamePlay/GameStatics.h"
#include "GamePlay/PlayerSpectatorPawn.h"
#include "GamePlay/RTSGameMode.h"
#include "Helpers/GridHelper.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/ObjectsManager.h"

UHologramPoolingComponent::UHologramPoolingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHologramPoolingComponent::Initialize(FObjectsManager* InitObjectsManager, UWorld* InitWorld, UMaterial* InitNotAcceptableMaterial)
{
	ActiveId = 0;
	ObjectsManager = InitObjectsManager;
	World = InitWorld;
	NotAcceptableMaterial = InitNotAcceptableMaterial;

	TMap<int32, FBuildingInfo>& Buildings = GameStatics::GetBuildings();
	for (auto Row : Buildings)
	{
		FBuildingInfo Data = Row.Value;
		ABaseHolograms* Hologram = World->SpawnActor<ABaseHolograms>(Data.HologramClass, FTransform(FRotator::ZeroRotator, FVector::Zero()));
		Holograms.Add(Row.Key, &*Hologram);
	}
	BuildingConstructComponent = MakeUnique<FBuildingConstructComponent>(ObjectsManager);
}

void UHologramPoolingComponent::ResetActiveHologram()
{
	Holograms.FindChecked(ActiveId)->ResetHologram();
	ActiveId = 0;
}

void UHologramPoolingComponent::SetActiveId(uint32 NewActiveId)
{
	if (ActiveId != 0)
	{
		ResetActiveHologram();
	}
	ActiveId = NewActiveId;
}

void UHologramPoolingComponent::ShowBuilding()
{
	Holograms.FindChecked(ActiveId)->Show();
}

bool UHologramPoolingComponent::CanPlaceOn(const FVector2d& Location)
{
	const FBuildingInfo* BuildingInfo = GameStatics::GetBuildingReferenceByID(ActiveId);
	return BuildingConstructComponent->CanPlaceOn(BuildingInfo, Location);
}

void UHologramPoolingComponent::SetNotAcceptedMaterial()
{
	Holograms.FindChecked(ActiveId)->SetNewMaterial(NotAcceptableMaterial);
}

void UHologramPoolingComponent::SetNormalMaterial()
{
	Holograms.FindChecked(ActiveId)->SetNormalMaterial();
}

void UHologramPoolingComponent::MoveTo(const FVector& Location)
{
	Holograms.FindChecked(ActiveId)->SetActorLocation(Location);
}

void UHologramPoolingComponent::StartBuilding(const FVector2d& Location, ETeams Team)
{
	if (!CastChecked<APlayerSpectatorPawn>(GetOwner())->GetComponentByClass<UPlayerResourceManagerComponent>()->CanPay(GameStatics::GetBuildingReferenceByID(ActiveId)->GetPrice()))
	{
		return;
	}
	FBuildingInfo* BuildingInfo = GameStatics::GetBuildingReferenceByID(ActiveId);
	if (BuildingConstructComponent->CanPlaceOn(BuildingInfo, Location))
	{
		FVector2d Grid = GridHelper::GetLocationOnGridCornerFromVector(Location, ObjectsManager->GetCellSize());
		BuildingConstructComponent->Build(World, FVector(Grid, 0), BuildingInfo, Team);
	}
}
