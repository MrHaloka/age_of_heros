#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GamePlay/PlayerSpectatorPawn.h"
#include "ScreenScrollComponent.generated.h"
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AOE2_API UScreenScrollComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UScreenScrollComponent();
protected:
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
	void Scroll();
	float MovementSpeed;
	const FMatrix2x2 RotationMatrix  = FMatrix2x2(0.866f, 0.5f, -0.5f, 0.866f);
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
