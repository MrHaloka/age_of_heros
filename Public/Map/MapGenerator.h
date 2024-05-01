#pragma once

#include "CoreMinimal.h"
#include "Actors/Units/BaseUnit.h"

class FObjectsManager;

class FMapGenerator
{
public:
	explicit FMapGenerator(FObjectsManager& DefaultObjectsManager);
	~FMapGenerator();
public:
	void AddAllDefaultsToMiniMap(ARTSHUD* RTSHUD) const;
protected:
	FObjectsManager& ObjectsManager;
	TSet<AActor*> DefaultActors;
	UWorld* World;
	void CreateTownCenter();
	void CreateStartingVillagers();
	void CreateResources();
	void CreateHouses();
};
