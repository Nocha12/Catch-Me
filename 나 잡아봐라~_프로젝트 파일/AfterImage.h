#pragma once
#include "Entity.h"
#include "Sprite.h"

class AfterImage : public Entity
{
public:
	AfterImage(Sprite *sprite);
	~AfterImage();

	Sprite *sprite;
	int speed;

	void update(float dt);
};

