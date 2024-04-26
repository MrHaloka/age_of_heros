#include "Pathing/ThetaStar.h"

#include <queue>
#include "Components/LineOfSightComponent.h"
#include "DataStructures/PathLinkedList.h"
#include "DataStructures/ThetaVertex.h"
#include "Helpers/GridHelper.h"
#include "Iterators/GridInLineIterator.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/ObjectsManager.h"

FThetaStar::FThetaStar(FObjectsManager* ObjectsManager, FVector2d Start, FVector2d End, const uint16 CollisionRadius) :
ObjectsManager(ObjectsManager),  EndGridId(ObjectsManager->GetGridId(End)), EndingNode(End), StartingNode(Start), CollisionRadius(CollisionRadius)
,EndingGrid(GridHelper::GetLocationOnGridCornerFromVector(End))
{
	LineOfSightComponent = MakeUnique<FLineOfSightComponent>(ObjectsManager);
	AddToVertexes(GridHelper::GetLocationOnGridCornerFromVector(StartingNode), 0);
	OpenIDs.push(std::pair(TNumericLimits<double>::Max(), ObjectsManager->GetGridId(StartingNode)));
}

FThetaStar::~FThetaStar()
{
}

FPathLinkedList* FThetaStar::CalculateThePath()
{
	while (!OpenIDs.empty())
	{
		const uint32 NewPoppedGridId = OpenIDs.top().second;
		OpenIDs.pop();
		FThetaVertex& CurrentVertex = Vertexes.FindChecked(NewPoppedGridId);
		if (NewPoppedGridId == EndGridId)
		{
			break;
		}
		
		Closes.Add(CurrentVertex.GridId);
		for (const uint32 NeighbourId : ObjectsManager->GetAllUnblockedAdjacentNeighboursIds(CurrentVertex.GetLocation()))
		{
			if (!Closes.Contains(NeighbourId))
			{
				if (!Opens.Contains(NeighbourId))
				{
					AddToVertexes(ObjectsManager->GetGridFrom(NeighbourId));
				}
				UpdateVertex(Vertexes.FindChecked(NeighbourId), CurrentVertex);
			}
		}
	}
	//For now just to stop it from crashing whenever there's no accessible path
	if (!Vertexes.Contains(EndGridId))
	{
		FPathLinkedList* Path =  new FPathLinkedList;
		Path->SetPath(FVector2d::Zero());
		return Path;
	}
	FPathLinkedList* Path = LoadThePath();
	PostProcess(Path);
	return Path;
}

/**
 * Create and add a new vertex to the Vertexes list.
 * @param VertexGridLocation the location of new vertex on corner of grid.
 * @param G the cost of the path from the starting node.
 */
void FThetaStar::AddToVertexes(const FVector2d& VertexGridLocation, double G)
{
	uint32 GridId = ObjectsManager->GetGridId(VertexGridLocation);
	FThetaVertex NewVertex = FThetaVertex(
		VertexGridLocation,
		G,
		FVector2d::Distance(VertexGridLocation, EndingGrid),
		TNumericLimits<uint32>::Max(),
		GridId
	);
	Vertexes.Add(GridId, NewVertex);
}

/**
 * Calculating and updating the G value if the new path to a vertex via its new parent is shorter.
 * @param Vertex the vertex for which we want to calculate the G value.
 * @param Parent the new parent that we are using for this vertex.
 */
void FThetaStar::CalculateAndUpdateG(FThetaVertex& Vertex, FThetaVertex& Parent)
{
	// First path : standard A* calculation of the G value
	double NewG = Parent.G + FVector2d::Distance(Vertex.GetLocation(), Parent.GetLocation());
	if (NewG < Vertex.G)
	{
		Vertex.G = NewG;
		Vertex.Parent = Parent.GridId;
	}
	// Second path : updating the parent based on the line of sight and subsequently calculating the G value
	check(Vertex.Parent != TNumericLimits<uint32>::Max())
	FThetaVertex ParentVertex = Vertexes.FindChecked(Vertex.Parent);
	if (ParentVertex.Parent == TNumericLimits<uint32>::Max())
	{
		return;
	}
	FThetaVertex ParentParentVertex = Vertexes.FindChecked(ParentVertex.Parent);
	if (LineOfSightComponent->HasLineOfSight(FGridInLineIterator(Vertex.GetLocation(), ParentParentVertex.GetLocation(), ObjectsManager->GetCellSize())))
	{
		NewG = ParentParentVertex.G + FVector2d::Distance(Vertex.GetLocation(), ParentParentVertex.GetLocation()) - 10;
		if (NewG <= Vertex.G)
		{
			Vertex.G = NewG;
			Vertex.Parent = ParentParentVertex.GridId;
		}
	}
}

/**
 * Call @link CalculateAndUpdateG() For the vertex with a new parent, its priority on the list will be updated if the cost (G value) decreases.
 * @param Vertex The vertex for which we want to evaluate the potential new parent path.
 * @param NewParent the new parent that we are using for this vertex.
 */
void FThetaStar::UpdateVertex(FThetaVertex& Vertex, FThetaVertex& NewParent)
{
	double OldG = Vertex.G;
	CalculateAndUpdateG(Vertex, NewParent);
	if (Vertex.G < OldG)
	{
		OpenIDs.push(std::pair(Vertex.GetF(), Vertex.GridId));
		Opens.Add(Vertex.GridId);
	}
}

/**
 * Starting from the end node, recursively traverse through its parents to load the path.
 * @return 
 */
FPathLinkedList* FThetaStar::LoadThePath()
{
	FPathLinkedList* Path = new FPathLinkedList(EndingNode);
	const uint16 HalfCell = ObjectsManager->GetCellSize() / 2 ;
	uint32 LastVertexId = Vertexes.FindChecked(EndGridId).Parent;
	while (LastVertexId != TNumericLimits<uint32>::Max())
	{
		FThetaVertex LastVertex = Vertexes.FindChecked(LastVertexId);
		FVector2d Location = LastVertex.GetLocation() + HalfCell;
		FPathLinkedList* NewPath = new FPathLinkedList(Location);
		Path->AddAfter(NewPath);
		Path = NewPath;
		LastVertexId = LastVertex.Parent;
	}
	if (Path->GetPrevious() != nullptr)
	{
		FPathLinkedList* OldPath = Path;
		Path = Path->GetPrevious();
		delete OldPath;
	}
	return Path;
}

/**
 * Process the path to account for actor collision radius.
 * @param Path The starting path which recursively traverses its parents to load the path from start to end.
 * It uses the previous node as the parent.
 */
void FThetaStar::PostProcess(FPathLinkedList* Path)
{
	FVector2d CurrentPath = Path->GetPath();
	Path = Path->GetPrevious();
	while (Path != nullptr)
	{
		FVector2d PreviousPath = Path->GetPath();
		FVector2d DirectionNormal = (PreviousPath - CurrentPath).GetSafeNormal();
		//two additional points for left and right side collision detection at the beginning and end
		FVector2d RightStart = CurrentPath + FVector2d(-DirectionNormal.Y, DirectionNormal.X) * CollisionRadius;
		FVector2d RightEnd = PreviousPath + FVector2d(-DirectionNormal.Y, DirectionNormal.X) * CollisionRadius;
		FVector2d LeftStart = CurrentPath + FVector2d(DirectionNormal.Y, -DirectionNormal.X) * CollisionRadius;
		FVector2d LeftEnd = PreviousPath + FVector2d(DirectionNormal.Y, -DirectionNormal.X) * CollisionRadius;
		//Retrieve all the actors that are obstructing the line of sight for these two new points(left and right).
		TArray<FVector2d> BlockedObjects = LineOfSightComponent->GetBlockingObjects(FGridInLineIterator(RightStart, RightEnd, PostProcessPrecision));
		BlockedObjects.Append(LineOfSightComponent->GetBlockingObjects(FGridInLineIterator(LeftStart, LeftEnd, PostProcessPrecision)));
		
		TSet<FVector2d> NewPaths = ChangePathForCollisionSize(BlockedObjects, CurrentPath, PreviousPath);
		for (FVector2d NewPath : NewPaths)
		{
			AddCollisionAvoidancePointsToPath(Path->GetNext(), NewPath);
		}

		CurrentPath = PreviousPath;
		Path = Path->GetPrevious();
	}
}

/**
 * Create a set of new nodes if necessary to prevent any collisions between the start and end points.
 * @param BlockedObjects The array of objects that obstructing the line of sight.
 * @param PathStart The location of starting node.
 * @param PathEnd The location of ending node.
 * @return The set of new nodes sorted by their distance to the start node, to be added to the path later.
 */
TSet<FVector2d> FThetaStar::ChangePathForCollisionSize(TArray<FVector2d>& BlockedObjects, const FVector2d& PathStart, const FVector2d& PathEnd)
{
	TSet<FVector2d> Results;
	
	for (FVector2d BlockedObject : BlockedObjects)
	{
		// Update the BlockedObject to the grid location of that object.
		BlockedObject = GridHelper::GetLocationOnGridCornerFromVector(BlockedObject);
		FVector2d NewPath = CalculateTheCollisionFreePoint(BlockedObject, PathStart, PathEnd);
		Results.Add(NewPath);
	}
	Results.Sort([&PathStart](const FVector2d& First, const FVector2d& Second)-> bool
		{
			return FVector2d::DistSquared(First, PathStart) < FVector2d::DistSquared(Second, PathStart);
		});
	return Results;
}

/**
 * receives the blocking point and calculates the 45-degree collision-free point outside the blocking grid that is closest to the path.
 * First, it identifies the farthest corner of the square from the line, then utilizes it to find the nearest corner to the line
 * (both closest and farthest to the line are on the same diameter)
 * @param BlockingGridLocation The location of blocking grid.
 * @param Start The start of the line.
 * @param End The end of line.
 * @return Closest collision-free point to the blocking point.
 */
FVector2d FThetaStar::CalculateTheCollisionFreePoint(const FVector2d& BlockingGridLocation, const FVector2d& Start, const FVector2d& End)
{	
	const float SQRT2INV = 0.7; // Representing the 45-degree vector's normal as FVector(0.7, 0.7) where each component is approximately 1.0/sqrt(2).
	FVector2d NormalVector = FVector2d(SQRT2INV, SQRT2INV);
	FVector2d Edge = BlockingGridLocation + FVector2d(100, 100);
	float MaxDistance = ProximityDistanceToLine(BlockingGridLocation, Start, End);
	float GridRight = ProximityDistanceToLine(FVector2d(BlockingGridLocation.X + 100, BlockingGridLocation.Y), Start, End) ;
	float GridTop = ProximityDistanceToLine(FVector2d(BlockingGridLocation.X, BlockingGridLocation.Y + 100), Start, End);
	float GridTopRight = ProximityDistanceToLine(FVector2d(BlockingGridLocation.X + 100, BlockingGridLocation.Y + 100), Start, End);
	if (GridRight > MaxDistance)
	{
		MaxDistance = GridRight;
		NormalVector = FVector2d(-SQRT2INV, SQRT2INV);
		Edge = BlockingGridLocation + FVector2d(0, 100);
	}
	if (GridTop > MaxDistance)
	{
		MaxDistance = GridTop;
		NormalVector = FVector2d(SQRT2INV, -SQRT2INV);
		Edge = BlockingGridLocation + FVector2d(100, 0);
	}
	if (GridTopRight > MaxDistance)
	{
		NormalVector = FVector2d(-SQRT2INV, -SQRT2INV);
		Edge = BlockingGridLocation + FVector2d(0, 0);
	}
	return Edge + NormalVector * CollisionRadius;
}

/**
 * Calculates the proximity distance of a point to a line segment defined by two other points.
 * It's based on calculating the distance from a point to a line defined by two other points formula.
 * @param Point The point for which we want to calculate the distance to the line.
 * @param Start The starting point of the line.
 * @param End The ending point of the line.
 * @return Float value representing the proximity of the point to the line segment.
 */
float FThetaStar::ProximityDistanceToLine(FVector2D Point, FVector2d Start, FVector2d End)
{
	return FMath::Abs((((End.X - Start.X) * (Start.Y - Point.Y)) - ((Start.X - Point.X) * (End.Y - Start.Y))));
}

/**
 * Add the new point after the path, or update the node if these two points are too close.
 * @param Path The node that we want to add a new point after it.
 * @param NewPoint The new node that we're adding to the path.
 */
void FThetaStar::AddCollisionAvoidancePointsToPath(FPathLinkedList* Path, const FVector2d& NewPoint)
{
	if (FVector2d::DistSquared(Path->GetPath(), NewPoint) < 4000)
	{
		Path->SetPath(NewPoint);
		return;
	}
	if (FVector2d::DistSquared(Path->GetPrevious()->GetPath(), NewPoint) < 4000)
	{
		Path->GetPrevious()->SetPath(NewPoint);
		return;
	}
	FPathLinkedList* NewLinkedListPoint = new FPathLinkedList(NewPoint);
	Path->AddBefore(NewLinkedListPoint);
}


