#pragma once
#include "entity.h"
#include "Timer.h"
class Scene :
	public Entity
{
public:
	Scene(void);
	~Scene(void);
	void update(float dt);
	void follow(Entity* target, Vec2 offset = Vec2(1280/2, 720/2));
	void unfollow();
	void shake(int n, float range, float duration);

	Entity* followTarget;
	Vec2 offset;
	Timer shakeTimer;
};
