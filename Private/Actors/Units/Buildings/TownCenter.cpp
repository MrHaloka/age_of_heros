#include "Actors/Units/Buildings/TownCenter.h"
#include "UI/RTSHUD.h"

ATownCenter::ATownCenter() : ABaseBuilding()
{
}

void ATownCenter::ShowUnitPanel(ARTSHUD* TargetHUD)
{
	Super::ShowUnitPanel(TargetHUD);
	TargetHUD->ShowTownCenterPanel(this);
}
