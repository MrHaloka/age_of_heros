#include "Map/LandscapeGenerator.h"
#include "Landscape.h"
#include "DataStructures/MapGeneration/HillData.h"
#include "DataStructures/MapGeneration/LandScapeData.h"
#include "GamePlay/GameStatics.h"

FLandscapeGenerator::FLandscapeGenerator(UWorld* InWorld, const TArray<FHillData>& Hills, const FVector& LandScapeScale, const TObjectPtr<UMaterialInterface>& LandscapeMaterial) :
	World(InWorld)
{
	check(World != nullptr)
	GenerateLandScape(Hills, LandScapeScale, LandscapeMaterial);
}

/**
 * Creates, spawns, and stores the landscape with the given data.
 * @param Hills The array of HillData used to generate hills on the landscape.
 * @param LandScapeScale The 3D scale of the landscape.
 * @param LandscapeMaterial The default material of the landscape.
 * TODO:: Landscape data needs to be imported from the map script.
 */
void FLandscapeGenerator::GenerateLandScape(const TArray<FHillData>& Hills, const FVector& LandScapeScale, const TObjectPtr<UMaterialInterface>& LandscapeMaterial)
{
	FLandScapeData LandScapeData(Hills, 63, 1, 2, 2);
	
	Landscape = World->SpawnActor<ALandscape>(ALandscape::StaticClass(), FTransform::Identity);
	Landscape->bCanHaveLayersContent = false;
	Landscape->LandscapeMaterial = LandscapeMaterial;
	Landscape->SetActorScale3D(LandScapeScale);
	Landscape->Import(FGuid::NewGuid(),
		0,
		0,
		LandScapeData.SizeX - 1,
		LandScapeData.SizeY - 1,
		LandScapeData.SectionsPerComponent,
		LandScapeData.QuadsPerComponent,
		LandScapeData.HeightDataPerLayers,
		nullptr,
		LandScapeData.MaterialLayerDataPerLayers,
		ELandscapeImportAlphamapType::Additive
		);
	Landscape->StaticLightingLOD = FMath::DivideAndRoundUp(FMath::CeilLogTwo((LandScapeData.SizeX * LandScapeData.SizeY) / (2048 * 2048) + 1), StaticCast<uint32>(2));
	ULandscapeInfo* LandscapeInfo = Landscape->GetLandscapeInfo();
	LandscapeInfo->UpdateLayerInfoMap(Landscape);
	Landscape->RegisterAllComponents();
	FPropertyChangedEvent MaterialPropertyChangedEvent(FindFieldChecked<FProperty>(Landscape->GetClass(), FName("LandscapeMaterial")));
	Landscape->PostEditChangeProperty(MaterialPropertyChangedEvent);
	Landscape->PostEditChange();
}

/**
 * Calculates the height of the 2D location on the hills.
 * @param LocationOnLandscape The 2D location on the landscape.
 * @return The height of the given location (0 if it's outside the landscape).
 */
int32 FLandscapeGenerator::GetHeight(const FVector2d& LocationOnLandscape) const
{
	TOptional<float> Result = Landscape->GetHeightAtLocation(FVector(LocationOnLandscape, 0));
	return Result.IsSet() ? Result.GetValue() : 0;
}