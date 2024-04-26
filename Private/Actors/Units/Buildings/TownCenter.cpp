#include "Actors/Units/Buildings/TownCenter.h"
#include "Kismet/GameplayStatics.h"
#include "UI/RTSHUD.h"

ATownCenter::ATownCenter()
{
}

void ATownCenter::ShowUnitPanel(ARTSHUD* TargetHUD)
{
	Super::ShowUnitPanel(TargetHUD);
	TargetHUD->ShowTownCenterPanel(this);
}
