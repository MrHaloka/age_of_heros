#include "Components/ToolsComponent.h"

#include "Actors/Units/MoveableUnit.h"
#include "Components/ResourceHarvestingComponent.h"
#include "DataStructures/ResourcesInfo.h"
#include "Enums/Tool.h"

UToolsComponent::UToolsComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	ActiveTool = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tool Mesh"));
}

void UToolsComponent::BeginPlay()
{
	Super::BeginPlay();
	check(GetOwner()->IsA(AMoveableUnit::StaticClass()))
	StaticCast<AMoveableUnit*>(GetOwner())->GetPrepareUnitStateEventHandler().AddUFunction(this, "OwnerPrepareToStateChangeListener");
	ActiveTool->AttachToComponent(&StaticCast<AMoveableUnit*>(GetOwner())->GetSkeletalMeshComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "Weapon_Socket_R");
}

void UToolsComponent::SetNewActiveTool(ETool ToolType)
{
	if (ToolType == ActiveToolType)
	{
		return;
	}
	ActiveToolType = ToolType;
	ActiveTool->SetStaticMesh(Tools.FindChecked(ToolType));
	ActiveTool->SetVisibility(true);
}

void UToolsComponent::OwnerPrepareToStateChangeListener(TEnumAsByte<EUnitState> NewState, TEnumAsByte<EUnitState> OldState)
{
	if (NewState == Harvesting || NewState == Moving_To_Resources)
	{
		const EResource ResourceType = GetOwner()->GetComponentByClass<UResourceHarvestingComponent>()->GetResourceInfo()->Type;
		if (ResourceType == EResource::Wood )
		{
			SetNewActiveTool(ETool::Axe);
		}
		else if ((ResourceType == EResource::Stone || ResourceType == EResource::Gold))
		{
			SetNewActiveTool(ETool::PickAxe);
		}
	}
	else if(NewState == Building || NewState == Moving_To_Building)
	{
		SetNewActiveTool(ETool::Hammer);
	}
	else
	{
		ActiveTool->SetVisibility(false);
		ActiveToolType = NullOpt;
	}
}
