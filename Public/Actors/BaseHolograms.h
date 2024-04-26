#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseHolograms.generated.h"

UCLASS()
class AOE2_API ABaseHolograms : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseHolograms();
	void Show();
	void ResetHologram();
	void SetNewMaterial(UMaterial* NewMaterial);
	void SetNormalMaterial();
protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* MainMesh;
	UPROPERTY()
	UMaterial* DefaultMaterial;
};
