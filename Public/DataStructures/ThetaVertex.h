#pragma once
#include "CoreMinimal.h"
#include "ThetaVertex.generated.h"

USTRUCT()
struct FThetaVertex
{
	GENERATED_BODY()

public:
	FVector2d GridLocation;
	double G;
	double H;
	uint32 Parent;
	uint32 GridId;
	
	FThetaVertex() : G(TNumericLimits<double>::Max()), H(TNumericLimits<double>::Max()), Parent(TNumericLimits<uint32>::Max())
	{
	}

	FThetaVertex(const FVector2d& GridLocation, const double G, const double H, uint32 Parent, uint32 GridId) :
	GridLocation(GridLocation), G(G), H(H), Parent(Parent), GridId(GridId)
	{
	}

	FVector2d GetLocation() const
	{
		return GridLocation;
	}

	double GetF()
	{
		return G + H;
	}
};
