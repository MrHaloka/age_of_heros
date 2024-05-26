#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ToolsComponent.generated.h"

UCLASS( Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AOE2_API UToolsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UToolsComponent();

protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly, Category="Tools|Meshes")
	TMap<ETool, UStaticMesh*> Tools;
protected:
	TOptional<ETool> ActiveToolType;
	UPROPERTY()
	UStaticMeshComponent* ActiveTool;
	void SetNewActiveTool(ETool ToolType);
	UFUNCTION()
	void OwnerPrepareToStateChangeListener(TEnumAsByte<EUnitState> NewState, TEnumAsByte<EUnitState> OldState);
};

