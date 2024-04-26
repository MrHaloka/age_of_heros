// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RTSPlayerController.generated.h"

class UInputComponent;
class UInputMappingContext;
class UInputAction;
UCLASS()
class AOE2_API ARTSPlayerController : public APlayerController
{
	GENERATED_BODY()
	ARTSPlayerController();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
};
