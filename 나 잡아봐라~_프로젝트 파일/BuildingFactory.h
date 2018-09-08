#pragma once
#include "Building.h"
#include "Scene.h"

class BuildingFactory
{
public:
	BuildingFactory(Scene *scene);
	~BuildingFactory();

	Building* MakeBuilding(ObjectType type, bool buildNow = false, bool spawnSide = false, TILE_CODE code = GROUND_TILE);

	Scene *gscene;
};

