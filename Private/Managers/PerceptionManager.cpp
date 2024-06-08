#include "Managers/PerceptionManager.h"

#include "Actors/Units/MoveableUnit.h"
#include "Components/AI/SightComponent.h"
#include "Containers/SpatialHash2d.h"
#include "Helpers/GridHelper.h"
#include "Managers/ObjectsManager.h"

FPerceptionManager::FPerceptionManager(FObjectsManager* InObjectsManager, uint8 GridSize, uint16 NewCellSize) :
CellSize(NewCellSize),
ObjectsManager(InObjectsManager)
{
	PerceptionGrid = MakeUnique<FSpatialHash2d>(GridSize, CellSize);
}

void FPerceptionManager::UnitAddedToGridListener(const ABaseUnit* AddedUnit, bool bIsHologram) const
{
	if (bIsHologram)
	{
		return;
	}
	for (uint32 UnitId : PerceptionGrid->GetPointsInGrid(AddedUnit->GetActorLocation2d()))
	{
		const ABaseUnit* Observer = ObjectsManager->GetUnits().FindChecked(UnitId);
		if (FVector2d::DistSquared(Observer->GetActorLocation2d(), AddedUnit->GetActorLocation2d()) > Observer->GetComponentByClass<USightComponent>()->GetPerceptionRadiusSquare())
		{
			continue;
		}
		Observer->GetComponentByClass<USightComponent>()->Sensed(AddedUnit);
	}
}

void FPerceptionManager::UnitMovedListener(AMoveableUnit* Moved, const FVector2D& NewLocation)
{
	UpdatePerceptionGrid(Moved, NewLocation);
	for (const uint32 UnitId : PerceptionGrid->GetPointsInGrid(Moved->GetActorLocation2d()))
	{
		if (UnitId == Moved->GetID())
		{
			continue;
		}
		const ABaseUnit* Unit = ObjectsManager->GetUnits().FindChecked(UnitId);
		if (FVector2d::DistSquared(Unit->GetActorLocation2d(), NewLocation) > Unit->GetComponentByClass<USightComponent>()->GetPerceptionRadiusSquare())
		{
			continue;
		}
		Unit->GetComponentByClass<USightComponent>()->Sensed(StaticCast<ABaseUnit*>(Moved));
	}
}

void FPerceptionManager::RegisterUnitSightPerception(const ABaseUnit* NewObserver)
{
	check(NewObserver->GetComponentByClass<USightComponent>() != nullptr)
	PerceptionGrid->InsertPoints(
		NewObserver->GetID(),
		NewObserver->GetComponentByClass<USightComponent>()->GetPerceptionCenter(),
		NewObserver->GetComponentByClass<USightComponent>()->GetPerceptionRadius()
		);
	const TSet<uint32> SensedUnits = ObjectsManager->GetUnitsInLocation(NewObserver->GetComponentByClass<USightComponent>()->GetPerceptionCenter(), NewObserver->GetComponentByClass<USightComponent>()->GetPerceptionRadius(), NewObserver->GetID());
	CheckSensedUnits(NewObserver, SensedUnits);
}

void FPerceptionManager::RemoveUnitSightPerception(const ABaseUnit* Unit)
{
	check(Unit->GetComponentByClass<USightComponent>() != nullptr)
	PerceptionGrid->RemovePointsUnsafe(
		Unit->GetID(),
		Unit->GetComponentByClass<USightComponent>()->GetPerceptionCenter(),
		Unit->GetComponentByClass<USightComponent>()->GetPerceptionRadius()
		);
}

ABaseUnit* FPerceptionManager::GetFirstEnemyInPerception(ABaseUnit* Observer)
{
	const TSet<uint32> SensedUnits = ObjectsManager->GetUnitsInLocation(Observer->GetComponentByClass<USightComponent>()->GetPerceptionCenter(), Observer->GetComponentByClass<USightComponent>()->GetPerceptionRadius(), Observer->GetID());
	for (uint32 UnitId : SensedUnits)
	{
		ABaseUnit* Unit = ObjectsManager->GetUnits().FindChecked(UnitId);
		if (Unit->GetTeam() == Observer->GetTeam() || FVector2d::DistSquared(Unit->GetActorLocation2d(), Observer->GetActorLocation2d()) > Observer->GetComponentByClass<USightComponent>()->GetPerceptionRadiusSquare())
		{
			continue;
		}
		return Unit;
	}
	return  nullptr;
}

void FPerceptionManager::NotifyObserver(const ABaseUnit* Observer, const uint32& SensedUnitId)
{
	if (SensedUnitId == Observer->GetID() || !ObjectsManager->GetUnits().Contains(SensedUnitId))
	{
		return;
	}
	Observer->GetComponentByClass<USightComponent>()->Sensed(ObjectsManager->GetUnits().FindChecked(SensedUnitId));
}

void FPerceptionManager::CheckSensedUnits(const ABaseUnit* Observer, const TSet<uint32>& SensedUnits)
{
	for (const uint32 SensedUnitId : SensedUnits)
	{
		NotifyObserver(Observer, SensedUnitId);
	}
}

void FPerceptionManager::CheckSensedUnits(const ABaseUnit* Observer, const TArray<uint32>& SensedUnits)
{
	for (const uint32 SensedUnitId : SensedUnits)
	{
		NotifyObserver(Observer, SensedUnitId);
	}
}

void FPerceptionManager::UpdatePerceptionGrid(AMoveableUnit* Moved, const FVector2D& NewLocation)
{
	if (Moved->GetComponentByClass<USightComponent>() == nullptr)
	{
		return;
	}
	FVector2d Delta = GridHelper::GetLocationOnGridCornerFromVector(NewLocation) - GridHelper::GetLocationOnGridCornerFromVector(Moved->GetActorLocation2d());
	if (Delta.X)
	{
		const int32 PerceptionRadius = Moved->GetComponentByClass<USightComponent>()->GetPerceptionRadius();
		const int32 NewAreaX = Delta.X > 0 ? NewLocation.X + PerceptionRadius : NewLocation.X - PerceptionRadius;
		const int32 OldAreaX = Delta.X > 0 ? Moved->GetActorLocation2d().X - PerceptionRadius : Moved->GetActorLocation2d().X + PerceptionRadius;
		for (int Y = -PerceptionRadius ; Y <= PerceptionRadius; Y += CellSize)
		{
			PerceptionGrid->UpdatePoint(Moved->GetID(), FVector2d(OldAreaX, Moved->GetActorLocation2d().Y + Y), FVector2d(NewAreaX, Moved->GetActorLocation2d().Y + Y));
			CheckSensedUnits(Moved, ObjectsManager->GetActorsInGrid(FVector2d(NewAreaX, Moved->GetActorLocation2d().Y + Y)));
		}
	}
	if (Delta.Y)
	{
		const int32 PerceptionRadius = Moved->GetComponentByClass<USightComponent>()->GetPerceptionRadius();
		const int32 NewAreaY = Delta.Y > 0 ? NewLocation.Y + PerceptionRadius :  NewLocation.Y - PerceptionRadius;
		const int32 OldAreaY = Delta.Y > 0 ? Moved->GetActorLocation2d().Y - PerceptionRadius : Moved->GetActorLocation2d().Y + PerceptionRadius;
		for (int X = -PerceptionRadius ; X <= PerceptionRadius; X += CellSize)
		{
			PerceptionGrid->UpdatePoint(Moved->GetID(), FVector2d(Moved->GetActorLocation2d().X +Delta.X + X, OldAreaY), FVector2d(Moved->GetActorLocation2d().X +Delta.X + X, NewAreaY));
			CheckSensedUnits(Moved, ObjectsManager->GetActorsInGrid(FVector2d(Moved->GetActorLocation2d().X + Delta.X + X, NewAreaY)));
		}
	}
}


