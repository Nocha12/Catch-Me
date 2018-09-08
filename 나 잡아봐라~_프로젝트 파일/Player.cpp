#include "DXUT.h"
#include "Player.h"
#include "MultiDirAnimation.h"
#include "World.h"
#include "GameScene.h"
#include "Arrow.h"
#include "Item.h"
#include "Asset.h"
#include "BottleTrap.h"

Player::Player() : timer(0), isBuilding(false), isCuttingTree(false), pickUpTrash(false), building(nullptr), pickingTimer(0), isPicking(false), waterEffectTimer(0), treeCuttingTimer(0), pickUpTrashTimer(0), throwSomething(false), throwTimer(0), throwed(false)
{
	type = PLAYER;

	addChild(animationMap[IDLE] = new MultiDirAnimation(this, "image/character/player/idle", 30, 30));
	rect = animationMap[IDLE]->rect;
	addChild(animationMap[WALK] = new MultiDirAnimation(this, "image/character/player/run", 18, 18));
	animationMap[WALK]->visible = false;
	addChild(animationMap[PICK] = new MultiDirAnimation(this, "image/character/player/pick", 20, 30));
	animationMap[PICK]->visible = false;
	addChild(animationMap[PICK_UP] = new MultiDirAnimation(this, "image/character/player/pickup", 20, 30));
	animationMap[PICK_UP]->visible = false;
	addChild(animationMap[AX] = new MultiDirAnimation(this, "image/character/player/ax", 20, 30));
	animationMap[AX]->visible = false;
	addChild(animationMap[SHOVEL] = new MultiDirAnimation(this, "image/character/player/shovel", 20, 30));
	animationMap[SHOVEL]->visible = false;
	addChild(animationMap[THROW] = new MultiDirAnimation(this, "image/character/player/throw", 25, 50));
	animationMap[THROW]->visible = false;
	addChild(animationMap[FALL_DOWN] = new MultiDirAnimation(this, "image/character/player/falldown", 12, 6, false));
	animationMap[FALL_DOWN]->visible = false;

	collisionPos = Vec2(126, 173); 
	colRange = 50;
	moveSpeed = 300;
	attackFrame = 3;
	attackRange = 180;
}

Player::~Player()
{
}

void Player::render()
{
	Unit::render();

	for_iter(iter, animationMap)
		(*iter).second->render();
}

void Player::update(float dt)
{
	Unit::update(dt);

	GameScene *scene = (GameScene*)world.currentScene;

	if (animationMap[FALL_DOWN]->visible)
	{
		scene->ui->black->color.a += dt * 0.2f;
		if (scene->ui->black->color.a >= 1)
		{
			scene->goToEnding = true;
			asset.getSound(L"sound/Ingame/Ingame_farming.wav")->Stop();
			asset.getSound(L"sound/Ingame/Ingame_phase1.wav")->Stop();
			asset.getSound(L"sound/Ingame/Ingame_phase2.wav")->Stop();
		}
	}

	if (world.getKeyState(VK_LBUTTON) == 1 && canMove)
	{
		if (scene->ui->selectToolSprite[BOTTLE_TRAP_TOOL]->visible || scene->ui->selectToolSprite[SLINGSHOT_TOOL]->visible || (scene->ui->selectToolSprite[BOMB_TOOL]->visible && scene->waitBuild != nullptr && scene->waitBuild->canBuild))
		{
			routeList.clear();
			float dir = angle(GetCollisionPos(), world.getMousePos() - scene->pos);
			moveRotation = dir;
			finalMovePos = GetCollisionPos();
			throwSomething = true;
			throwTimer = 0;
			canMove = false;
		}
		if (scene->ui->selectToolSprite[WATER_TOOL]->visible && !afterImageTimer.active)
		{
			asset.getSound(L"sound/Item_Using/item_4.wav")->Reset();
			asset.getSound(L"sound/Item_Using/item_4.wav")->Play();

			afterImageTimer.reset(0.2f, 25);
			moveSpeed *= 1.5f;
			afterImageTimer.onTick = [=]()
			{
				Sprite *sprite = new Sprite();

				for_iter(iter, animationMap)
				{
					if ((*iter).second)
					{
						MultiDirAnimation *ani = (MultiDirAnimation*)(*iter).second;
						if (ani->visible)
							sprite->setTexture(ani->animation[ani->direction]->texture);
					}
				}
				
				AfterImage *img = new AfterImage(sprite);
				scene->afterImageList.push_back(img);
				world.currentScene->addChild(img);
				img->pos = pos;
			};
			afterImageTimer.onFinished = [=]()
			{
				moveSpeed /= 1.5f;
			};

			scene->ui->makedItemCount[WATER_TOOL]--;
		}
	}

	if (throwSomething)
	{
		throwTimer += dt;
		if (throwTimer > 0.25f && !throwed)
		{
			throwed = true;
			if (scene->ui->selectToolSprite[BOTTLE_TRAP_TOOL]->visible)
			{
				float dir = angle(GetCollisionPos(), world.getMousePos() - scene->pos);
				moveRotation = dir;
				BottleTrap *trap = new BottleTrap(dir);
				scene->addChild(trap);
				scene->objList.push_back(trap);
				trap->SetCollisionPos(GetCollisionPos() + (Vec2(cos(dir), sin(dir) * 40)));
				scene->ui->makedItemCount[BOTTLE_TRAP_TOOL]--;
				asset.getSound(L"sound/Item_Using/item_7.wav")->Reset();
				asset.getSound(L"sound/Item_Using/item_7.wav")->Play();
			}
			if (scene->ui->selectToolSprite[SLINGSHOT_TOOL]->visible)
			{
				float dir = angle(GetCollisionPos() - Vec2(0, 70), world.getMousePos() - scene->pos);
				moveRotation = dir;
				Arrow *a = new Arrow(dir, 600, 5);
				scene->addChild(a);
				scene->arrowList.push_back(a);
				a->setCenter(GetCollisionPos() - Vec2(0, 70));
				scene->ui->makedItemCount[SLINGSHOT_TOOL]--;
				asset.getSound(L"sound/Item_Using/item_3.wav")->Reset();
				asset.getSound(L"sound/Item_Using/item_3.wav")->Play();
			}
			if (scene->ui->selectToolSprite[BOMB_TOOL]->visible)
			{
				building->ScaleUp();
				building->Build(dt);
				scene->ui->makedItemCount[BOMB_TOOL]--;
				canMove = false;
				asset.getSound(L"sound/Item_Using/item_5.wav")->Reset();
				asset.getSound(L"sound/Item_Using/item_5.wav")->Play();
			}
		}

		if (throwTimer > 0.5f)
		{
			throwed = false;
			canMove = true;
			throwSomething = false;
		}
		ChangeAnimation(THROW);
	}

	if (!canMove && routeList.empty() && isBuilding)
	{
		if (building->type == GRASS_TRAP || building->type == WATER_TRAP || building->type == TOUCH)
		{
			building->buildingDelay = 0.4;
			building->Build(dt);
			ChangeAnimation(PICK_UP);
		}
	}

	if (!canMove && routeList.empty() && pickUpTrash && attackTarget)
	{
		pickUpTrashTimer += dt;

		if (pickUpTrashTimer > 1.3f)
		{
			asset.getSound(L"sound/Ingame/Hit_glass.wav")->Play(); 

			pickUpTrashTimer = 0;
			pickUpTrash = false;
			canMove = true;
			for (int i = 0; i < random(1, 2); i++)
			{
				Item *item = new Item(BOTTLE_ITEM);
				scene->ui->makeItemList.push_back(item);
			}
			scene->ui->MakeItem(attackTarget->GetCollisionPos());

			((Building*)attackTarget)->onDead();
		}
		ChangeAnimation(SHOVEL);
	}
	else if (!canMove && routeList.empty() && isPicking && attackTarget)
	{
		pickingTimer += dt;

		if (pickingTimer > 1.3f)
		{
			asset.getSound(L"sound/Ingame/Hit_stone.wav")->Play();

			pickingTimer = 0;
			isPicking = false;
			canMove = true;
			for (int i = 0; i < random(1, 2); i++)
			{
				Item *item = new Item(ROCK_ITEM);
				scene->ui->makeItemList.push_back(item);
			}
			scene->ui->MakeItem(attackTarget->GetCollisionPos());

			((Building*)attackTarget)->onDead();
		}
		ChangeAnimation(PICK);
	}
	else if (!canMove && routeList.empty() && isCuttingTree && attackTarget)
	{
		treeCuttingTimer += dt;

		if (treeCuttingTimer > 1.3f)
		{
			asset.getSound(L"sound/Ingame/Hit_wood.wav")->Play();

			treeCuttingTimer = 0;
			isCuttingTree = false;
			canMove = true;
			for (int i = 0; i < random(1, 3); i++)
			{
				Item *item = new Item(BRANCH_ITEM);
				scene->ui->makeItemList.push_back(item);
			}
			for (int i = 0; i < random(1, 3); i++)
			{
				Item *item = new Item(LEAF_ITEM);
				scene->ui->makeItemList.push_back(item);
			}
			for (int i = 0; i < random(1, 2); i++)
			{
				Item *item = new Item(SAP_ITEM);
				scene->ui->makeItemList.push_back(item);
			}
			scene->ui->MakeItem(attackTarget->GetCollisionPos());

			((Building*)attackTarget)->onDead();
		}

		ChangeAnimation(AX);
	}

	if (currentTile->code == WATER_TILE && waterEffectTimer >= 0.3f)
	{
		waterEffectTimer = 0;
		Sprite *spr = new Sprite("image/circle.png");
		spr->scaleCenter = spr->rect.center();
		addChild(spr);
		spr->setCenter(collisionPos);
		waterEffectList.push_back(spr);
	}
	else if (currentTile->code != WATER_TILE)
	{
		for_iter(iter, waterEffectList)
			(*iter)->visible = false;
	}
	waterEffectTimer += dt;
	for_iter(iter, waterEffectList)
	{
		(*iter)->scale += Vec2(dt * 2, dt * 2);
		(*iter)->color.a -= dt;
		if ((*iter)->color.a <= 0)
		{
			removeChild(*iter);
			iter = waterEffectList.erase(iter);
			if (iter == waterEffectList.end()) break;
		}
	}
}
