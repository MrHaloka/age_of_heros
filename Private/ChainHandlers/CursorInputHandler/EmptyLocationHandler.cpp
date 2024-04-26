#include "ChainHandlers/CursorInputHandler/EmptyLocationHandler.h"

#include "ChainHandlers/CursorInputHandler/ResourceHandler.h"
#include "Commands/MoveCommand.h"


FEmptyLocationHandler::FEmptyLocationHandler(TSet<ABaseUnit*>& Units, TUniquePtr<FResourceHandler> ResourceHandler) :
Units(Units), NextHandler(MoveTemp(ResourceHandler))
{
}

void FEmptyLocationHandler::Execute(AActor* NewSelectedUnit, const FVector2d& Location)
{
	if (NewSelectedUnit != nullptr)
	{
		NextHandler->Execute(NewSelectedUnit);
		return;
	}
	FMoveCommand MoveCommand(Units, Location);
	MoveCommand.RunCommand();
}
 