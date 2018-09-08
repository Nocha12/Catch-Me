#pragma once
#include "Object.h"
class BottleTrap :
	public Object
{
public:
	BottleTrap(float dir);
	~BottleTrap();

	float dir;
	float timer;

	Entity *animation;
	float moveSpeed;

	void update(float dt);
};

