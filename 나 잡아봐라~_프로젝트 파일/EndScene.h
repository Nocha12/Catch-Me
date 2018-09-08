#pragma once
#include "Scene.h"
#include "Animation.h"

class EndScene :
	public Scene
{
public:
	EndScene(bool isClear);
	~EndScene();
	
	Sprite *black;
	Animation *ed;
	Sprite *spr;
	bool isClear;

	void update(float dt);
};

