#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseResources.generated.h"

enum class ETeams;
struct FResourcesInfo;
UCLASS()
class AOE2_API ABaseResources : public AActor
{
	GENERATED_BODY()

public:	
	ABaseResources();
	ETeams GetTeam();
	FColor GetMiniMapColor();
	void SetID(uint32 ID);
	uint32 GetID() const;
	void RemoveFromResource(float Amount);
	FVector2d GetActorLocation2d() const;
	int32 GetResourceId() const;
	UPROPERTY(EditDefaultsOnly, Category="Mesh")
	UStaticMeshComponent* StaticMesh;
	UPROPERTY(EditDefaultsOnly, Category="Resource Type")
	int32 ResourceId;
	const FResourcesInfo* GetResourceInfo() const;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
protected:
	virtual void BeginPlay() override;
	uint32 ID;
	float ResourceAvailable;
	const FResourcesInfo* ResourcesInfo;
};
