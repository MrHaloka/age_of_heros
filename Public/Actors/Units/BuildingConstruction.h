#pragma once

#include "CoreMinimal.h"
#include "Components/BuildingConstructComponent.h"
#include "Enums/Teams.h"
#include "Actors/Units/BaseUnit.h"
#include "BuildingConstruction.generated.h"

struct FBuildingInfo;
UCLASS()
class AOE2_API ABuildingConstruction : public ABaseUnit
{
	GENERATED_BODY()
public:
	ABuildingConstruction();
	virtual FVector2d GetSize() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
	virtual float GetMaxHP() override;
	virtual FResources GetPrice() override;
	void AddVillagerToBuild(UBuilderComponent* BuilderComponent);
	void RemoveVillagerFromBuilding(UBuilderComponent* BuilderComponent, const FVector2d& BuilderLocation);
	void UpdateBuildingProgress(float DeltaSeconds);
	void Finish();
	void Refund();
	void Initialize(const FBuildingInfo* NewBuildingInfo, ETeams NewPlayerTeam);
	const FBuildingInfo* GetBuildingInfo() const;
	bool IsStarted();
	void Starting();
	TOptional<FVector2d> GetClosestEmptyBuilderLocation(const FVector2d& BuilderLocation);
protected:
	TSet<UBuilderComponent*> ActiveBuilders;
	TSet<FVector2d> BuildersGridLocations;
	uint8 VillagerCounter = 0;
	const FBuildingInfo* BuildingInfo;
	bool BStarted = false;
	double TimeToComplete;
	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	UStaticMeshComponent* HologramMesh;
	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	UStaticMeshComponent* UnderConstructionMesh;
	void SpawnTheBuilding();
	void CalculateBuildersLocations();
	void BroadcastBuildingCompletion();
};
