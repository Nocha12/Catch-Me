#pragma once
#include "Entity.h"
#include "Scene.h"

class StageMap :
	public Entity
{
public:
	StageMap(Scene* scene);
	~StageMap();

	void init();
	void mapCrop(int tileCode);

	Scene* scene;
	int tiles[TILECOUNT][TILECOUNT];
};

