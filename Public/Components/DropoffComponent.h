#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DropoffComponent.generated.h"


struct FSingleResource;
class ABaseUnit;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AOE2_API UDropoffComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UDropoffComponent();
	~UDropoffComponent();
	void SetDropoffType(EResource Resource);
	EResource GetResourceType() const;
	void SetID(uint32 NewID);
	uint32 GetID() const;
	FVector2d GetGridLocation();
	ABaseUnit* GetBaseUnitOwner();
	void DroppedResources(FSingleResource Resource);
protected:
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
	EResource ResourceType;
	uint32 ID;
public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
