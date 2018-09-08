#include "DXUT.h"
#include "Item.h"
#include "World.h"
#include "GameScene.h"
#include "Asset.h"

Item::Item(ItemType type) : type(type), speed(600), isFollow(false)
{
	Sprite *spr = nullptr;

	if (type == LEAF_ITEM)
		addChild(spr = new Sprite("image/item/Drop/leaf.png"));
	if (type == ROCK_ITEM)
		addChild(spr = new Sprite("image/item/Drop/rock.png"));
	if (type == BRANCH_ITEM)
		addChild(spr = new Sprite("image/item/Drop/stem.png"));
	if (type == BOTTLE_ITEM)
		addChild(spr = new Sprite("image/item/Drop/bottle.png"));
	if (type == SAP_ITEM)
		addChild(spr = new Sprite("image/item/Drop/sap.png"));

	moveRotation = D3DXToRadian(random(0, 360));

	rect = spr->rect;
	speed += random(-100, 50);
}

Item::~Item()
{
}

void Item::update(float dt)
{
	if (removing)
		return;

	Entity::update(dt);
	GameScene *scene = (GameScene*)world.currentScene;

	goalPos = scene->ui->inven->pos + rect.center();

	pos += Vec2(cos(moveRotation), sin(moveRotation)) * dt * speed;
	
	if (speed <= 10 && !isFollow)
	{
		moveRotation = angle(center(), goalPos - scene->pos);
		speed = 1700;
		isFollow = true;
	}
	if (!isFollow)
		speed -= dt * speed * 8;
	else
	{
		moveRotation = angle(center(), goalPos - scene->pos);
		speed -= dt * speed;
		if (speed < 300)
			speed = 300;
	}

	if (pita(goalPos - scene->pos, center()) < 50)
	{
		asset.getSound(L"sound/Ingame/Item_pickup.wav")->Reset();
		asset.getSound(L"sound/Ingame/Item_pickup.wav")->Play();

		scene->itemList.remove(this);
		removeChild(this);
		scene->ui->itemCount[type]++;
	}
}
