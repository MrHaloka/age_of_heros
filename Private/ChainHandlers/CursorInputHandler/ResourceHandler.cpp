#include "ChainHandlers/CursorInputHandler/ResourceHandler.h"

#include "ChainHandlers/CursorInputHandler/EnemyUnitHandler.h"
#include "Commands/VillagerHarvestCommand.h"
#include "Actors/Resources/BaseResources.h"

FResourceHandler::FResourceHandler(TSet<ABaseUnit*>& Units, TUniquePtr<FEnemyUnitHandler> EnemyUnitHandler):
Units(Units), NextHandler(MoveTemp(EnemyUnitHandler))
{
}

void FResourceHandler::Execute(AActor* Resource)
{
	if (!Resource->IsA(ABaseResources::StaticClass()))
	{
		NextHandler->Execute(reinterpret_cast<ABaseUnit*>(Resource));
		return;
	}
	VillagerHarvestCommand HarvestCommand(Units, reinterpret_cast<ABaseResources*>(Resource));
	HarvestCommand.RunCommand();
}
