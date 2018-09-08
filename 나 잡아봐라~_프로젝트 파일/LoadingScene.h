#pragma once
#include "Animation.h"
#include "Scene.h"
class LoadingScene :
	public Scene
{
public:
	LoadingScene();
	~LoadingScene();

	void update(float dt);

	Sprite *bar;
};

