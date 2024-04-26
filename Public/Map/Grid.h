// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grid.generated.h"

class UGridDrawer;
UCLASS()
class AOE2_API AGrid : public AActor
{
	GENERATED_BODY()

public:
	AGrid();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere)
	UGridDrawer* GridDrawer;
};
