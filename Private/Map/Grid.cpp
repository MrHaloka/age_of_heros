#include "Map/Grid.h"
#include "Map/GridDrawer.h"

// Sets default values
AGrid::AGrid()
{
	PrimaryActorTick.bCanEverTick = false;
	GridDrawer = CreateDefaultSubobject<UGridDrawer>(TEXT("MapGridDrawer"));
	AddOwnedComponent(GridDrawer);
}

void AGrid::BeginPlay()
{
	Super::BeginPlay();
}

void AGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
