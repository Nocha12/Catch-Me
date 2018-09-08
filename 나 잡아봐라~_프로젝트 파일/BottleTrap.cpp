#include "DXUT.h"
#include "BottleTrap.h"
#include "MultiDirAnimation.h"
#include "GameScene.h"
#include "World.h"
#include "Follower.h"

BottleTrap::BottleTrap(float dir) : moveSpeed(350), dir(dir), timer(15)
{
	type = BOTTLE_TRAP;
	addChild(animation = new MultiDirAnimation(this, "image/item/InGame/bottle", 20, 30));
	rect = animation->rect; 

	collisionPos = Vec2(76, 68);
	colRange = 0;
	moveRotation = dir + D3DXToRadian(90);
	collisionAble = false;
}


BottleTrap::~BottleTrap()
{
}

void BottleTrap::update(float dt)
{
	GameScene *scene = (GameScene*)world.currentScene;
	Entity::update(dt);

	for_iter(iter, scene->objList)
	{
		if (((*iter)->isUnit && (*iter)->type != PLAYER))
		{
			if (circleCollision(center(), (*iter)->GetCollisionPos(), 70))
			{
				((Follower*)(*iter))->FallDown(nullptr);
				for_iter(i, ((Unit*)*iter)->animationMap)
					((MultiDirAnimation*)i->second)->GetRed();

				scene->objList.remove(this);
				removeChild(this);
				return;
			}
		}
	}

	pos += Vec2(cos(dir), sin(dir)) * moveSpeed * dt;
	timer -= dt;

	if (timer <= 0)
	{
		scene->objList.remove(this);
		removeChild(this);
	}
}