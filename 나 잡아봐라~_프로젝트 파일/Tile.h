#pragma once
#include "Entity.h"
#include "Sprite.h"

class Object;

enum TILE_CODE{
	DEEP_WATER_TILE,
	WATER_TILE,
	SAND_TILE,
	GROUND_TILE
};

class Tile :
	public Entity
{
public:
	Tile(int code);
	~Tile();

	Sprite* spr;
	Sprite* waterSpr;
	int code;

	Vec2 GetCenter();
	int GetAllCost(Tile *target);
	int GetAllParentCost();
	int GetParentCost(Tile *p);

	Object* target;
	Vec2 tilePos;
	Tile* parentTile;
	bool blocked;
	bool unitTile;
	bool canBuild;
	bool buildTile;

	float count;

	void update(float dt);
};

