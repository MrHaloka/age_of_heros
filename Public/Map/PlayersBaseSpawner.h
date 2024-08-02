#pragma once

#include "CoreMinimal.h"

struct FPlayerBaseData;
class FObjectsManager;
/**
 * Creates base buildings, units, and starting points for players.
 */
class AOE2_API FPlayersBaseSpawner
{
public:
	FPlayersBaseSpawner(FObjectsManager& ObjectsManager, const FPlayerBaseData& PlayerBaseData, const FVector2d& MapMaxLocation, const FRandomStream& RandomStream);
	bool IsInPlayerSafeArea(const FVector2d& Location) const;
	FVector2d GetFirstPlayerLocation() const;
	FVector2d GetSecondPlayerLocation() const;

protected:
	FVector2d FirstPlayerLocation;
	FVector2d SecondPlayerLocation;
	const int32 SafeAreaSqr;
};
