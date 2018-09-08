#include "DXUT.h"
#include "Particle.h"
#include "World.h"
#include "GameScene.h"

Particle::Particle(path p, float dir, float speed) : dir(dir), speed(speed)
{
	addChild(sprite = new Sprite(p));
	sprite->setCenter(center());
	sprite->rotationCenter = sprite->rect.center();
}

Particle::~Particle()
{
}

void Particle::update(float dt)
{
	Entity::update(dt);

	speed -= dt * speed * 2;
	sprite->color.a -= dt * 1.5f;

	if (sprite->color.a <= 0)
	{
		((GameScene*)world.currentScene)->particleList.remove(this);
		removeChild(this);
	}

	pos += Vec2(cos(dir), sin(dir)) * dt * speed;
	
	sprite->rotation += dt * 10;
}