#pragma once

struct FPlayerBaseData
{
	FPlayerBaseData(float InMinPlayersDistance, float InCenterRadius, float InSafeArea = 0):
		MinPlayersDistance(InMinPlayersDistance),
		CenterRadius(InCenterRadius),
		SafeArea(InSafeArea)
	{
	}
	FPlayerBaseData(): MinPlayersDistance(0), CenterRadius(0), SafeArea(0)
	{
	}

	float MinPlayersDistance;
	float CenterRadius;
	float SafeArea;
};
