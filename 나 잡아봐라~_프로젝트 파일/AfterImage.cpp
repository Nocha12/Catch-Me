#include "DXUT.h"
#include "AfterImage.h"
#include "World.h"
#include "GameScene.h"

AfterImage::AfterImage(Sprite *sprite) : sprite(sprite), speed(2)
{
	addChild(sprite);
	sprite->color.a = 0.7f;
}

AfterImage::~AfterImage()
{
}

void AfterImage::update(float dt)
{
	Entity::update(dt);
	sprite->color.a -= dt * speed;
	if (sprite->color.a <= 0)
	{
		removeChild(this);
		((GameScene*)world.currentScene)->afterImageList.remove(this);
	}
}