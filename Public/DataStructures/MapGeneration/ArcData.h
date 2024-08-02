#pragma once
#include "CoreMinimal.h"
#include "ArcData.generated.h"

USTRUCT(BlueprintType)
struct FArcData
{
	GENERATED_BODY()

public:
	FArcData(float StartingAngel, float AngularLength): StartingAngel(StartingAngel), AngularLength(AngularLength)
	{
	}

	FArcData(): StartingAngel(0.0f), AngularLength(TWO_PI)
	{
	}
	
	/**
	* Compute the arc parameters given the center and two intersection points.
	* @param CenterLocation The location of the center of the circle.
	* @param FirstPoint The location of the first intersection point of the circle with the map.
	* @param SecondPoint The location of the second intersection point of the circle with the map.
	*/
	FArcData(const FVector2D& CenterLocation, const FVector2d& FirstPoint, const FVector2d& SecondPoint)
	{
		const FVector2d FirstDirectionNormal = (FirstPoint - CenterLocation).GetSafeNormal();
		const FVector2d SecondDirectionNormal = (SecondPoint - CenterLocation).GetSafeNormal();
		float FirstAngle =  FMath::Atan2(FirstDirectionNormal.Y, FirstDirectionNormal.X);
		float SecondAngle =  FMath::Atan2(SecondDirectionNormal.Y, SecondDirectionNormal.X);
		//normalize to 0 - TWO_PI
		FirstAngle = FMath::Fmod(FirstAngle + TWO_PI, TWO_PI);
		SecondAngle = FMath::Fmod(SecondAngle + TWO_PI, TWO_PI);
		const float Distance = FMath::Abs(FirstAngle - SecondAngle);
		if (Distance > TWO_PI - Distance)
		{
			StartingAngel = FMath::Min(FirstAngle, SecondAngle);
			AngularLength = Distance;
			return;
		}
		StartingAngel = FMath::Max(FirstAngle, SecondAngle);
		AngularLength = TWO_PI - Distance;
	}
	
	float StartingAngel;
	float AngularLength;
};
