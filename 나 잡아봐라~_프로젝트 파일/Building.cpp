#include "DXUT.h"
#include "Building.h"
#include "GameScene.h"
#include "World.h"
#include "Asset.h"

Building::Building() : buildTile(nullptr), buildMode(true), canBuild(false), sprite(nullptr), animation(nullptr), buildingTimer(0), fade(1), buildingDelay(0)
{
}

Building::~Building()
{
}

void Building::update(float dt)
{
	Object::update(dt);
	GameScene* scene = (GameScene*)world.currentScene;

	Vec2 mousePos = world.getMousePos() - scene->pos;

	if (buildMode)
	{
		sprite->pos = -collisionPos + Vec2(TILEWIDTH, TILEHEIGHT / 2);
		sprite->color.a = 0.5f;
		buildTile = scene->GetTile(mousePos);
		pos = buildTile->pos;
		
		CheckCanBuild();
	}
}

void Building::CheckCanBuild() 
{
	GameScene *scene = (GameScene*)world.currentScene;
	
	int cnt = 0;

	for_iter(check, checkList)
	{
		Tile *tile = scene->GetTile(pos + (*check)->center());

		if (type == WATER_TRAP &&(tile->blocked || tile->unitTile || tile->buildTile || tile->code != WATER_TILE))
		{
			(*check)->setPath("image/map/red.png");
			cnt++;
		}
		else if (type == WATER_TRAP) (*check)->setPath("image/map/green.png");

		else if (tile->blocked || tile->unitTile || tile->buildTile || !tile->canBuild || tile->spr->color == D3DXCOLOR(194.0f / 256, 170.0f / 256, 149.0f / 256, 1))
		{
			(*check)->setPath("image/map/red.png");
			cnt++;
		}
		else (*check)->setPath("image/map/green.png");
	}

	if (cnt == 0) canBuild = true;
	else canBuild = false;
}

void Building::CheckCanBuild(TILE_CODE code)
{
	GameScene *scene = (GameScene*)world.currentScene;

	int cnt = 0;

	for_iter(check, checkList)
	{
		Tile *tile = scene->GetTile(pos + (*check)->center());

		if (tile->blocked || tile->unitTile || tile->buildTile || !tile->canBuild || tile->code != code || tile->spr->color == D3DXCOLOR(194.0f / 256, 170.0f / 256, 149.0f / 256, 1))
		{
			(*check)->setPath("image/map/red.png");
			cnt++;
		}
		else (*check)->setPath("image/map/green.png");
	}

	if (cnt == 0) canBuild = true;
	else canBuild = false;
}

void Building::instantBuild()
{
	GameScene *scene = (GameScene*)world.currentScene;

	currentTile = buildTile;
	
	collisionPos = Vec2(TILEWIDTH * 0.5f + (width - 1) * TILEWIDTH * 0.25f + (height - 1) * TILEWIDTH * 0.25f,
						TILEHEIGHT * 0.5f - (width - 1) * TILEHEIGHT * 0.25f + (height - 1) * TILEHEIGHT * 0.25f);

	for_iter(iter, checkList)
	{
		scene->GetTile(pos + (*iter)->center())->buildTile = true;
		(*iter)->visible = false;
	}
	buildMode = false;

	scene->player->SetWalk(currentTile->GetCenter());
	scene->player->canMove = false;
	scene->player->isBuilding = true;
	scene->player->building = this;

	sprite->color.a = 0.5f;
}

void Building::instantBuild(bool setBuildTile, bool isMove)
{
	GameScene *scene = (GameScene*)world.currentScene;

	currentTile = buildTile;

	collisionPos = Vec2(TILEWIDTH * 0.5f + (width - 1) * TILEWIDTH * 0.25f + (height - 1) * TILEWIDTH * 0.25f,
		TILEHEIGHT * 0.5f - (width - 1) * TILEHEIGHT * 0.25f + (height - 1) * TILEHEIGHT * 0.25f);

	for_iter(iter, checkList)
	{
		scene->GetTile(pos + (*iter)->center())->canBuild = false;
		(*iter)->visible = false;
	}
	buildMode = false;

	if (isMove)
	{
		scene->player->SetWalk(currentTile->GetCenter());
		scene->player->canMove = false;
		scene->player->isBuilding = true;
	}
	scene->player->building = this;

	sprite->color.a = 0.5f;
}


void Building::onDead()
{
	type = DELETEING;
	GameScene *scene = (GameScene*)world.currentScene;
	for_iter(iter, scene->objList)
	{
		if ((*iter)->attackTarget == this)
			(*iter)->attackTarget = nullptr;
	}
	for_iter(iter, checkList)
	{
		scene->GetTile(pos + (*iter)->center())->buildTile = false;
		scene->GetTile(pos + (*iter)->center())->canBuild = true;
		removeChild(*iter);
	}

	FadeOut();
}

void Building::setCheckTile()
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			Sprite *t;
			t = new Sprite("image/map/green.png");
			t->pos = (Vec2(x * TILEWIDTH / 2 + y * TILEWIDTH / 2, -x * TILEHEIGHT / 2 + y * TILEHEIGHT / 2));
			t->color.a = 0.5f;
			addChild(t);
			checkList.push_back(t);
		}
	}
	buildMode = true;
}

void Building::FadeOut()
{
	GameScene *scene = (GameScene*)world.currentScene;
	fadeOutTimer.reset(0.01f, 25);
	fadeOutTimer.onTick = [=]()
	{
		fade -= 0.04f;
		if (animation)
			animation->color = D3DXCOLOR(1, 1, 1, fade);
		if (sprite)
			sprite->color = D3DXCOLOR(1, 1, 1, fade);
	};
	fadeOutTimer.onFinished = [=](){
		removeChild(this);
		scene->objList.remove(this); 
		for_iter(iter, scene->objList)
		{
			if ((*iter)->attackTarget == this)
				(*iter)->attackTarget = nullptr;
		}
	};
}

void Building::Build(float dt)
{
	GameScene *scene = (GameScene*)world.currentScene;

	buildingTimer += dt;

	if (buildingTimer > buildingDelay)
	{
		sprite->color.a = 1;
		scene->player->canMove = true;
		scene->player->isBuilding = false;
		scene->player->building = nullptr;

		if (type == GRASS_TRAP)
			asset.getSound(L"sound/Item_Using/item_2.wav")->Play();
		if (type == WATER_TRAP)
			asset.getSound(L"sound/Item_Using/item_6.wav")->Play();

		if (type == TOUCH)
		{
			asset.getSound(L"sound/Item_Using/item_8.wav")->Play();
			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 6; j++)
				{
					Tile *t = scene->mapTile[(int)currentTile->tilePos.x - 1 + i][(int)currentTile->tilePos.y - 3 + j];
					if (t->code != WATER_TILE && t->code != DEEP_WATER_TILE)
						t->spr->color = D3DXCOLOR(1, 166 / (float)255, 142 / (float)255, 1);
				}
		}
	}
}

void Building::ScaleUp()
{
	scaleCenter = collisionPos;
	scale = Vec2(0, 0);
	scaleTimer.reset(0.01, 20);
	scaleTimer.onTick = [=](){
		scale = Vec2(lerp(scale.x, 1, scaleTimer.count.first / (float)scaleTimer.count.second), lerp(scale.y, 1, scaleTimer.count.first / (float)scaleTimer.count.second));
	};
}