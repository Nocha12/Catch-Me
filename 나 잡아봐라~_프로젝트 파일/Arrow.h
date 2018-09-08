#pragma once
#include "Entity.h"
#include "Sprite.h"

class Arrow :
	public Entity
{
public:
	Arrow(float dir, float speed, float timer);
	~Arrow();

	float dir;
	float speed;
	float timer;
	float fade;

	Sprite *sprite;

	void update(float dt);
};

