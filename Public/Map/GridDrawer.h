// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DataStructures/GridData.h"
#include "GridDrawer.generated.h"


struct FGridData;
class UProceduralMeshComponent;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AOE2_API UGridDrawer : public UActorComponent
{
	GENERATED_BODY()

public:	
	UGridDrawer();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UPROPERTY(EditAnywhere)
	FGridData GridData;
	UProceduralMeshComponent* ProceduralMeshComponent;
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	void CreateHorizontalAndVerticalLine(const float LineStart, const float LineEnd);
	void CreateGrid();
	void CreateLine(FVector3d Start, FVector3d End, float& Thickness, TArray<FVector>& Out_Vertices, TArray<int32>& Out_Triangles);
};
