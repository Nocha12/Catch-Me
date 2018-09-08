#pragma once
#include "Scene.h"
#include "Animation.h"

class MainScene :
	public Scene
{
public:
	MainScene();
	~MainScene();

	Sprite *gameStartB;
	Sprite *exitB;
	Sprite *howToB;

	Sprite *howToEx[5];
	Sprite *resultEx;
	Sprite *title;
	Sprite *cursor;
	
	float moveTitle;
	int howToCount;

	void update(float dt);
};

