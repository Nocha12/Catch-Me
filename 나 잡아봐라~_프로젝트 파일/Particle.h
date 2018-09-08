#pragma once
#include "Entity.h"
#include "Sprite.h"

class Particle :
	public Entity
{
public:
	Particle(path p, float dir, float speed);
	~Particle();

	Sprite *sprite;
	float dir;
	float speed;

	void update(float dt);
};

