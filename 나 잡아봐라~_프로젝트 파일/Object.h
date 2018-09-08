#pragma once
#include "Entity.h"
#include "Tile.h"

class Object :
	public Entity
{
public:
	Object();
	~Object();

	ObjectType type;

	Vec2 collisionPos;
	Vec2 GetCollisionPos();
	void SetCollisionPos(Vec2 pos);

	Object *attackTarget;
	Tile *currentTile;
	float moveRotation;

	bool isUnit;
	int colRange;
	bool collisionAble;
};

