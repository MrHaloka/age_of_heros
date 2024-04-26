#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DataStructures/Resources.h"
#include "PlayerResourceManagerComponent.generated.h"

class ARTSHUD;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AOE2_API UPlayerResourceManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPlayerResourceManagerComponent();
public:
	void AddNewResources(FResources NewResource);
	void RemoveResources(FResources RemoveResources);
	FResources GetResources() const;
	void CallResourcePanelForUpdate();
	bool Pay(const FResources& Price);
	void Refund(const FResources& RefundResources);
	bool CanPay(const FResources& Price);
	void AddNewResources(const FSingleResource& Resource);
protected:
	virtual void BeginPlay() override;
	FResources Resources;
	UPROPERTY()
	ARTSHUD* RTSHUD;
};
