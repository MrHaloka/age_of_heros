#pragma once

class ABaseUnit;
enum class ETeams;
struct FBuildingInfo;
class FObjectsManager;

class AOE2_API FBuildingConstructComponent
{
public:
	FBuildingConstructComponent(FObjectsManager* ObjectsManager);
	bool CanPlaceOn(const FBuildingInfo* BuildingInfo, const FVector2d Location) const;
	bool CanStartBuilding(const FBuildingInfo* BuildingInfo, const FVector2d Location, uint32 BuildingId) const;
	ABaseUnit* Build(UWorld* World, const FVector& Location, const FBuildingInfo* BuildingInfo, ETeams PlayerTeam);
protected:
	FObjectsManager* ObjectsManager;
};
