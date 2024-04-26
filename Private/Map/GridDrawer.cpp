#include "Map/GridDrawer.h"
#include "ProceduralMeshComponent.h"
#include "DataStructures/GridData.h"

UGridDrawer::UGridDrawer()
{
	PrimaryComponentTick.bCanEverTick = false;
	ProceduralMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GridProceduralMesh"));
}

void UGridDrawer::BeginPlay()
{
	Super::BeginPlay();
	CreateGrid();
}

void UGridDrawer::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UGridDrawer::CreateHorizontalAndVerticalLine(const float LineStart,const float LineEnd)
{
	CreateLine(FVector3d(LineStart, 0, 0),FVector3d(LineStart, LineEnd, 0),GridData.GridThickness, Vertices,Triangles);
	CreateLine(FVector3d(0, LineStart, 0),FVector3d(LineEnd, LineStart, 0),GridData.GridThickness, Vertices,Triangles);
}

void UGridDrawer::CreateGrid()
{
	for (int32 Counter = 0; Counter < GridData.GridSize; Counter ++)
	{
		const float LineStart = Counter * GridData.TileSize;
		const float LineEnd = GridData.GridSize * GridData.TileSize;
		CreateHorizontalAndVerticalLine(LineStart, LineEnd);
	}
	ProceduralMeshComponent->
		CreateMeshSection(0, Vertices, Triangles, TArray<FVector3d>(), TArray<FVector2D>(), TArray<FVector2D>(),
		                  TArray<FVector2D>(), TArray<FVector2D>(),
		                  TArray<FColor>(), TArray<FProcMeshTangent>(), false);
	ProceduralMeshComponent->SetCastShadow(false);
	ProceduralMeshComponent->SetMaterial(0, GridData.GridMaterial);
}

void UGridDrawer::CreateLine(FVector3d Start, FVector3d End, float& Thickness, TArray<FVector>& Out_Vertices,
                             TArray<int32>& Out_Triangles)
{
	FVector3d ThickNessDirection = (End - Start).GetSafeNormal().Cross(FVector3d::ZAxisVector);
	float HalfThickNess = Thickness / 2;
	int32 VerticesCount = Out_Vertices.Num();

	Out_Triangles.Append({
		VerticesCount + 2, VerticesCount + 1, VerticesCount + 0, VerticesCount + 2, VerticesCount + 3,
		VerticesCount + 1
	});
	Out_Vertices.Append({
		Start + ThickNessDirection * HalfThickNess, End + ThickNessDirection * HalfThickNess,
		Start - ThickNessDirection * HalfThickNess, End - ThickNessDirection * HalfThickNess
	});
}
