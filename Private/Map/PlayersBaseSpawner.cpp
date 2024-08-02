#include "Map/PlayersBaseSpawner.h"

#include "DataStructures/BuildingInfo.h"
#include "DataStructures/MapGeneration/PlayerBaseData.h"
#include "Enums/Teams.h"
#include "Factories/UnitInfoFactory.h"
#include "GamePlay/GameStatics.h"
#include "Managers/ObjectsManager.h"

/**
 * Spawns the player base area and sets the players' locations.
 * @param ObjectsManager The reference to the objects manager related to the map.
 * @param PlayerBaseData The player base area data loaded from the map script.
 * @param MapMaxLocation The maximum location on the map.
 * @param RandomStream The random stream generated based on the seed number.
 */
FPlayersBaseSpawner::FPlayersBaseSpawner(FObjectsManager& ObjectsManager, const FPlayerBaseData& PlayerBaseData, const FVector2d& MapMaxLocation, const FRandomStream& RandomStream):
	SafeAreaSqr(PlayerBaseData.SafeArea * PlayerBaseData.SafeArea)
{
	const float MinAngle = 2 * FMath::Asin(PlayerBaseData.MinPlayersDistance / (PlayerBaseData.CenterRadius * 2));
	const FVector2d MapCenter = MapMaxLocation/2;
	const float FirstPlayerAngle = RandomStream.RandRange(0.0f, TWO_PI);
	const float SecondPlayerAngle =  RandomStream.RandRange(FirstPlayerAngle + MinAngle, TWO_PI + FirstPlayerAngle - MinAngle);

	FirstPlayerLocation = MapCenter + FVector2D(PlayerBaseData.CenterRadius * FMath::Cos(FirstPlayerAngle),PlayerBaseData.CenterRadius * FMath::Sin(FirstPlayerAngle));
	SecondPlayerLocation = MapCenter + FVector2D(PlayerBaseData.CenterRadius * FMath::Cos(SecondPlayerAngle),PlayerBaseData.CenterRadius * FMath::Sin(SecondPlayerAngle));

	FBuildingInfo* BuildingInfo = GameStatics::GetBuildingReferenceByID(1);
	ObjectsManager.SpawnUnit(FVector(FirstPlayerLocation, 0),FUnitInfoFactory(BuildingInfo->UnitClass, FVector2d(BuildingInfo->SizeX, BuildingInfo->SizeY)),ETeams::Blue);
	ObjectsManager.SpawnUnit(FVector(SecondPlayerLocation, 0),FUnitInfoFactory(BuildingInfo->UnitClass,FVector2d(BuildingInfo->SizeX, BuildingInfo->SizeY)), ETeams::Red);
}

bool FPlayersBaseSpawner::IsInPlayerSafeArea(const FVector2d& Location) const
{
	return FVector2d::DistSquared(Location, FirstPlayerLocation) < SafeAreaSqr || FVector2d::DistSquared(Location, SecondPlayerLocation) < SafeAreaSqr;
}

FVector2d FPlayersBaseSpawner::GetFirstPlayerLocation() const
{
	return FirstPlayerLocation;
}

FVector2d FPlayersBaseSpawner::GetSecondPlayerLocation() const
{
	return SecondPlayerLocation;
}

