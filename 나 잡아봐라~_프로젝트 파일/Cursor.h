#pragma once
#include "Entity.h"
#include "Sprite.h"
#include "Object.h"
#include "Scene.h"

class Cursor :
	public Entity
{
public:
	Cursor();
	~Cursor();
	
	bool canUseCursor;

	void update(float dt);
};

