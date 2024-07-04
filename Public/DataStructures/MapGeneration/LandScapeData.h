#pragma once
#include "CoreMinimal.h"
#include "HillData.h"

/**
 * This structure handles all the necessary data needed for importing to the landscape.
 */
struct FLandScapeData
{
	FLandScapeData(): SectionSize(0), SectionsPerComponent(0), ComponentCountX(0), ComponentCountY(0), QuadsPerComponent(0), SizeX(0), SizeY(0)
	{
		HeightData.Init(32768, SizeX * SizeY);
	}

	FLandScapeData(const TArray<FHillData>& Hills, int32 InSectionSize, int32 InSectionsPerComponent, int32 InComponentCountX, int32 InComponentCountY)
	{
		SectionSize = InSectionSize;
		SectionsPerComponent = InSectionsPerComponent;
		ComponentCountX = InComponentCountX;
		ComponentCountY = InComponentCountY;
		QuadsPerComponent = SectionSize * SectionsPerComponent;
		SizeX = ComponentCountX * QuadsPerComponent + 1;
		SizeY = ComponentCountY * QuadsPerComponent + 1;
		
		// Initialize height data for hills
		HeightData.Init(32768, SizeX * SizeY);
		for (const FHillData HillData : Hills)
		{
			AddHill(HillData);
		}
		
		// Add height data to layer
		MaterialImportLayers.Reserve(0);
		HeightDataPerLayers.Add(FGuid(), MoveTemp(HeightData));
		MaterialLayerDataPerLayers.Add(FGuid(), MoveTemp(MaterialImportLayers));
	}
public:
	int32 SectionSize;
	int32 SectionsPerComponent;
	int32 ComponentCountX;
	int32 ComponentCountY;
	int32 QuadsPerComponent;
	int32 SizeX;
	int32 SizeY;
	TArray<uint16> HeightData;
	TArray<FLandscapeImportLayerInfo> MaterialImportLayers;
	TMap<FGuid, TArray<uint16>> HeightDataPerLayers;
	TMap<FGuid, TArray<FLandscapeImportLayerInfo>> MaterialLayerDataPerLayers;
protected:
	void AddHill(const FHillData& HillData)
	{
		for (const TPair<uint32, uint32> HillInfo: HillData.Hills)
		{
			HeightData[HillInfo.Key] = FMath::Min<uint16>((HeightData[HillInfo.Key] + HillInfo.Value), 33328);
		}
	}
};
