#pragma once
#include "CoreMinimal.h"
#include <queue>
#include <vector>

typedef std::pair<double, uint32> DataType;

class FLineOfSightComponent;
class FObjectsManager;
class FIndexPriorityQueue;
struct FPathLinkedList;
struct FThetaVertex;

class AOE2_API FThetaStar
{
public:
	FThetaStar(FObjectsManager* ObjectsManager, FVector2d Start, FVector2d End,	const uint16 CollisionRadius);
	~FThetaStar();
	FPathLinkedList* CalculateThePath();
protected:
	FObjectsManager* ObjectsManager;
	const uint32 EndGridId;
	FVector2d EndingNode;
	const FVector2d EndingGrid;
	FVector2d StartingNode;
	TUniquePtr<FLineOfSightComponent> LineOfSightComponent;
	//The first element represents the F value of the vertex, and the second element represents the grid ID of that vertex
	std::priority_queue<DataType, std::vector<DataType>, std::greater<DataType>> OpenIDs;
	TMap<uint32, FThetaVertex> Vertexes;
	TSet<uint32> Opens;
	TSet<uint32> Closes;
	void AddToVertexes(const FVector2d& VertexGridLocation, double G = TNumericLimits<double>::Max());
	void CalculateAndUpdateG(FThetaVertex& Vertex, FThetaVertex& Parent);
	void UpdateVertex(FThetaVertex& Vertex, FThetaVertex& NewParent);
	FPathLinkedList* LoadThePath();
	const uint16 CollisionRadius;
	void PostProcess(FPathLinkedList* Path);
	/**
	* The PostProcessPrecision determines the size of collisions that always will be detected by post process.
	*/
	const uint16 PostProcessPrecision = 20;
	TSet<FVector2d> ChangePathForCollisionSize(TArray<FVector2d>& BlockedObjects, const FVector2d& PathStart, const FVector2d& PathEnd);
	FVector2d CalculateTheCollisionFreePoint(const FVector2d& BlockingGridLocation, const FVector2d& Start, const FVector2d& End);
	float ProximityDistanceToLine(FVector2D Point, FVector2d Start, FVector2d End);
	void AddCollisionAvoidancePointsToPath(FPathLinkedList* Path, const FVector2d& NewPoint);
};
