#include "DXUT.h"
#include "BuildingFactory.h"
#include "GameScene.h"
#include "World.h"

BuildingFactory::BuildingFactory(Scene *scene) : gscene(scene)
{
}

BuildingFactory::~BuildingFactory()
{
}

Building* BuildingFactory::MakeBuilding(ObjectType type, bool buildNow, bool spawnSide, TILE_CODE code)
{
	GameScene *scene = (GameScene*)gscene;

	if (type == TREE)
	{
		Building *b = new Building();
		b->type = TREE;
		b->addChild(b->sprite = new Sprite("image/build/Tree.png"));
		b->rect = b->sprite->rect;
		b->width = 1;
		b->height = 1;
		b->setCheckTile();
		scene->waitBuild = b;
		b->colRange = 20;
		b->collisionPos = Vec2(80 + TILEWIDTH / 2, 170);
		b->rect = b->rect.offset(-b->collisionPos + Vec2(TILEWIDTH, TILEHEIGHT / 2));
		b->rect.cutOff(Rect(10, 16, 2, 6));

		if (!buildNow)
			return b;
	}

	if (type == ROCK)
	{
		Building *b = new Building();
		b->type = ROCK;
		b->addChild(b->sprite = new Sprite("image/build/Rock.png"));
		b->rect = b->sprite->rect;
		b->width = 2;
		b->height = 2;
		b->setCheckTile();
		scene->waitBuild = b;
		b->colRange = 10;
		b->collisionPos = Vec2(82, 117);
		b->rect = b->rect.offset(-b->collisionPos + Vec2(TILEWIDTH, TILEHEIGHT / 2));
		b->rect.cutOff(Rect(7, 17, 4, 20));

		if (!buildNow)
			return b;
	}

	if (type == TRASH)
	{
		Building *b = new Building();
		b->type = TRASH;
		b->addChild(b->sprite = new Sprite("image/build/Trash.png"));
		b->rect = b->sprite->rect;
		b->width = 1;
		b->height = 1;
		b->setCheckTile();
		scene->waitBuild = b;
		b->colRange = 10;
		b->collisionPos = Vec2(56 + TILEWIDTH / 2, 67);
		b->rect = b->rect.offset(-b->collisionPos + Vec2(TILEWIDTH, TILEHEIGHT / 2));
		b->rect.cutOff(Rect(7, 8, 2, 15));

		if (!buildNow)
			return b;
	}

	if (type == WATER_TRAP)
	{
		Building *b = new Building();
		b->type = WATER_TRAP;
		b->addChild(b->sprite = new Sprite("image/item/InGame/waterTrap.png"));
		b->rect = b->sprite->rect;
		b->width = 1;
		b->height = 1;
		b->setCheckTile();
		scene->waitBuild = b;
		b->colRange = 43;
		b->collisionPos = Vec2(71  +TILEWIDTH / 2, 43);

		b->rect = b->rect.offset(-b->collisionPos + Vec2(TILEWIDTH, TILEHEIGHT / 2));

		if (!buildNow)
			return b;
	}

	if (type == BOMB)
	{
		Building *b = new Building();
		b->type = BOMB;
		b->addChild(b->sprite = new Sprite("image/item/InGame/bomb.png"));
		b->rect = b->sprite->rect;
		b->width = 3;
		b->height = 3;
		b->setCheckTile();
		scene->waitBuild = b;
		b->colRange = 110;
		b->collisionPos = Vec2(160 - TILEWIDTH / 2, 118);

		b->rect = b->rect.offset(-b->collisionPos + Vec2(TILEWIDTH, TILEHEIGHT / 2));

		if (!buildNow)
			return b;
	}

	if (type == TOUCH)
	{
		Building *b = new Building();
		b->type = TOUCH;
		b->addChild(b->sprite = new Sprite("image/item/InGame/touch.png"));
		b->rect = b->sprite->rect;
		b->width = 1;
		b->height = 1;
		b->setCheckTile();
		scene->waitBuild = b;
		b->colRange = 0;
		b->collisionPos = Vec2(27 + TILEWIDTH / 2, 68);

		b->rect = b->rect.offset(-b->collisionPos + Vec2(TILEWIDTH, TILEHEIGHT / 2));

		if (!buildNow)
			return b;
	}

	if (type == GRASS_TRAP)
	{
		Building *b = new Building();
		b->type = GRASS_TRAP;
		b->addChild(b->sprite = new Sprite("image/item/InGame/grassTrap.png"));
		b->rect = b->sprite->rect;
		b->width = 1;
		b->height = 1;
		b->setCheckTile();
		scene->waitBuild = b;
		b->colRange = 30;
		b->collisionPos = Vec2(50 + TILEWIDTH / 2, 60);

		b->rect = b->rect.offset(-b->collisionPos + Vec2(TILEWIDTH, TILEHEIGHT / 2));

		if (!buildNow)
			return b;
	}

	if (buildNow)
	{
		Building *b = scene->waitBuild;
		scene->objList.push_back(b);
		b->buildMode = false;
		b->sprite->color.a = 1;
		b->sprite->pos = -b->collisionPos + Vec2(TILEWIDTH, TILEHEIGHT / 2);
		scene->player->canMove = true;
		scene->player->isBuilding = false;
		scene->player->building = nullptr;
		scene->waitBuild = nullptr;

		while (!b->canBuild)
		{
			if (spawnSide)
				b->buildTile = scene->GetRandomSideTile(code);
			else
				b->buildTile = scene->GetRandomTile(code);

			b->pos = b->buildTile->pos;
			b->CheckCanBuild(code);
		}

		b->currentTile = b->buildTile;

		b->collisionPos = Vec2(TILEWIDTH * 0.5f + (b->width - 1) * TILEWIDTH * 0.25f + (b->height - 1) * TILEWIDTH * 0.25f,
			TILEHEIGHT * 0.5f - (b->width - 1) * TILEHEIGHT * 0.25f + (b->height - 1) * TILEHEIGHT * 0.25f);

		for_iter(iter, b->checkList)
		{
			scene->GetTile(b->pos + (*iter)->center())->buildTile = true;
			(*iter)->visible = false;
		}

		return b;
	}
}
