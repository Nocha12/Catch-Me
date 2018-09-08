#pragma once
#include "Unit.h"
#include "Timer.h"

class Follower :
	public Unit
{
public:
	Follower();
	~Follower();

	void update(float dt);

	float timer;
	bool isStartFollow;

	Timer fallDownTimer;
	Timer changeColorTimer;
	Timer changeNormalTimer;

	float maxSpeed;
	float plusMaxSpeed;

	void FallDown(Object *obj);
};
