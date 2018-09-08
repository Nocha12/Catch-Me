#include "DXUT.h"
#include "Tile.h"

Tile::Tile(int code) : code(code), parentTile(nullptr), blocked(false), unitTile(false), target(nullptr), canBuild(true), buildTile(false), count(0), waterSpr(0)
{
	if (code == DEEP_WATER_TILE){
		spr = new Sprite("image/map/water2.png");
		spr->pos.y += 15;
		blocked = true;
		canBuild = false;
	}
	else if (code == WATER_TILE){
		spr = new Sprite("image/map/water.png");
		spr->pos.y += 15;
		canBuild = false;
	}
	else if (code == SAND_TILE){
		spr = new Sprite("image/map/sand.png");
	}
	else if (code == GROUND_TILE){
		spr = new Sprite("image/map/grass.png");
	}
	
	addChild(spr);
	rect = spr->rect;
}

Tile::~Tile()
{
}

void Tile::update(float dt)
{
	if (code == WATER_TILE || code == DEEP_WATER_TILE)
	{
		spr->pos.y = sin(count += dt) * 5 + 5;
	}
}

int Tile::GetParentCost(Tile *p)
{
	if (p->tilePos.x != tilePos.x){
		if (p->tilePos.y != tilePos.y){
			return 5;
		}
		else{
			return 8;
		}
	}
	else{
		if (abs(p->tilePos.y - tilePos.y) == 2){
			return 6;
		}
		else{
			return 5;
		}
	}
}

int Tile::GetAllParentCost()
{
	if (parentTile == nullptr) return 0;
	return parentTile->GetAllParentCost() + GetParentCost(parentTile);
}

int Tile::GetAllCost(Tile *target)
{
	return GetAllParentCost() + (int)abs(target->tilePos.y - tilePos.y) * 5;
}

Vec2 Tile::GetCenter()
{
	return pos + Vec2(TILEWIDTH, TILEHEIGHT) / 2;
}