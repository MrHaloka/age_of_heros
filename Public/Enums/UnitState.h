#pragma once

UENUM()
enum EUnitState
{
	Idle UMETA(DisplayName = "Idle"),
	Moving UMETA(DisplayName = "Moving"),
	Moving_To_Resources UMETA(DisplayName = "Moving Towards Resources"),
	Moving_To_Building UMETA(DisplayName = "Moving Towards Building"),
	Building UMETA(DisplayName = "Building"),
	Harvesting UMETA(Displayname = "Gathering"),
	Moving_To_Dropoff UMETA(Displayname = "Moving Towards Dropoff")
};
