#pragma once
#include "Define.h"
#include "Entity.h"
#include "Scene.h"

class World
{
public:
	World(void);
	~World(void);
	void update(float dt);
	void render();
	void init();
	void dispose();
	void changeScene(Scene* scene);
	int getKeyState(int vk);
	Vec2 getMousePos();

	Scene* currentScene;
	LPD3DXSPRITE rootSprite;
	bool lastKeys[256];
	bool currentKeys[256];
	POINT mousePoint;
	list<Timer*> timers;
};

extern World world;