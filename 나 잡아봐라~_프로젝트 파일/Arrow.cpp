#include "DXUT.h"
#include "Arrow.h"
#include "GameScene.h"
#include "World.h"

Arrow::Arrow(float dir, float speed, float timer) : dir(dir), speed(speed), timer(timer)
{
	addChild(sprite = new Sprite("image/item/Drop/rock.png"));
	rect = sprite->rect;

	rotationCenter = sprite->rect.center();
	rotation = dir;
}

Arrow::~Arrow()
{
}

void Arrow::update(float dt)
{
	GameScene *scene = (GameScene*)world.currentScene;
	Entity::update(dt);

	for_iter(iter, scene->objList)
	{
		if (((*iter)->isUnit && (*iter)->type != PLAYER))
		{
			if (circleCollision(center(), (*iter)->GetCollisionPos(), (*iter)->colRange + 20))
			{
				((Unit*)*iter)->Attacked(dir, 500);
				scene->arrowList.remove(this);
				removeChild(this);
				return;
			}
		}
	}

	pos += Vec2(cos(dir), sin(dir)) * speed * dt;
	timer -= dt;

	if (timer <= 0)
	{
		sprite->color.a -= dt * 5;

		if (sprite->color.a <= 0)
		{
			scene->arrowList.remove(this);
			removeChild(this);
		}
	}
}