#pragma once
#include "Entity.h"
#include "Sprite.h"

class Item : public Entity
{
public:
	Item(ItemType type);
	~Item();

	ItemType type;

	float moveRotation;
	float speed;

	Vec2 goalPos;
	bool isFollow;

	void update(float dt);
};

