#pragma once

#include "CoreMinimal.h"
#include "Actors/Units/BaseUnit.h"

class FObjectsManager;

class FMapGenerator
{
public:
	explicit FMapGenerator(FObjectsManager& DefaultObjectsManager);
	~FMapGenerator();

protected:
	FObjectsManager& ObjectsManager;
	UWorld* World;
	void CreateTownCenter();
	void CreateStartingVillagers();
	void CreateResources();
	void CreateHouses();
};
