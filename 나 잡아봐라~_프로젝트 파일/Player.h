#pragma once
#include "Unit.h"
#include "Building.h"
#include "Timer.h"

class Player : public Unit
{
public:
	Player();
	~Player();

	void update(float dt);
	void render();

	bool isBuilding;
	bool isPicking;
	bool isCuttingTree;
	bool pickUpTrash;
	float pickingTimer;
	float treeCuttingTimer;
	float pickUpTrashTimer;

	int water;
	int sleep;
	int food;
	float timer;
	float waterEffectTimer;

	bool throwed;
	bool throwSomething;
	float throwTimer;

	bool isBlackOut;
	bool bowAttacked;
	bool crossBowAttacked;
	bool axAttacked;
	bool attackFin;

	Timer afterImageTimer;

	Building *building;
	Sprite *getWaterB;
	list<Sprite*> waterEffectList;
};

