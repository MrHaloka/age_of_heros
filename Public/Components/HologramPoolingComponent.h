#pragma once

#include "CoreMinimal.h"
#include "HologramPoolingComponent.generated.h"

class FBuildingConstructComponent;
class ABaseHolograms;
enum class ETeams;
class UDataTable;
class FObjectsManager;
/**
 * 
 */
UCLASS( ClassGroup=(Custom) )
class AOE2_API UHologramPoolingComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UHologramPoolingComponent();
	void Initialize(FObjectsManager* NewObjectsManager, UWorld* World, UMaterial* NotAcceptableMaterial);
	void ResetActiveHologram();
	void SetActiveId(uint32 NewActiveId);
	void ShowBuilding();
	bool CanPlaceOn(const FVector2d& Location);
	void SetNotAcceptedMaterial();
	void SetNormalMaterial();
	void MoveTo(const FVector& Location);
	void StartBuilding(const FVector2d& Location, ETeams Team);

protected:
	TMap<int32, ABaseHolograms*> Holograms = TMap<int32, ABaseHolograms*>();
	uint32 ActiveId;
	FObjectsManager* ObjectsManager;
	TUniquePtr<FBuildingConstructComponent> BuildingConstructComponent;
	UPROPERTY()
	UWorld* World;
	UPROPERTY()
	UMaterial* NotAcceptableMaterial;
};
