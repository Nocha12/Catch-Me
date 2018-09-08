#include "DXUT.h"
#include "Follower.h"
#include "MultiDirAnimation.h"
#include "World.h"
#include "GameScene.h"
#include "Asset.h"

Follower::Follower() : timer(0), isStartFollow(false), maxSpeed(50), plusMaxSpeed(0)
{
	type = FOLLOWER;

	addChild(animationMap[IDLE] = new MultiDirAnimation(this, "image/character/follower/idle", 30, 30));
	rect = animationMap[IDLE]->rect;
	addChild(animationMap[WALK] = new MultiDirAnimation(this, "image/character/follower/run", 18, 18));
	animationMap[WALK]->visible = false;
	addChild(animationMap[KNOCKBACK] = new MultiDirAnimation(this, "image/character/follower/knockback", 20, 20));
	animationMap[KNOCKBACK]->visible = false;
	addChild(animationMap[FALL_DOWN] = new MultiDirAnimation(this, "image/character/follower/falldown", 25, 10));
	animationMap[FALL_DOWN]->visible = false;

	rect.cutOff(Rect(60, 65, 60, 60));
	collisionPos = Vec2(126, 186);
	colRange = 50;
	attackRange = 180;
}

Follower::~Follower()
{
}


void Follower::update(float dt)
{
	Unit::update(dt);

	GameScene *scene = (GameScene*)world.currentScene;

	timer += dt;
		
	if (pita(scene->player->GetCollisionPos(), GetCollisionPos()) <= 120 && moveSpeed >= 100) 
	{
		if (!scene->cursor->canUseCursor)
		{
			scene->player->ChangeAnimation(FALL_DOWN);
			canMove = false;
			moveSpeed = 0;
			routeList.clear();
			return;
		}
		scene->player->stand = true;
		scene->player->ChangeAnimation(FALL_DOWN);
		scene->player->isBuilding = (false), scene->player->isCuttingTree = (false), scene->player->pickUpTrash = (false), scene->player->isPicking = (false), scene->player->throwSomething = false;
		scene->player->canMove = false;
		scene->cursor->canUseCursor = false;
		scene->player->routeList.clear();
		scene->player->moveSpeed = 0;
		routeList.clear();
		ChangeAnimation(IDLE);
		moveSpeed = 0;
		canMove = false;
		return;
	}

	if (!isStartFollow)
	{
		if (timer > 2.5f)
		{
			SetWalk(scene->player->GetCollisionPos());
			timer = 0;
			isStartFollow = true;
		}
		return;
	}

	if (canMove)
	{
		moveSpeed += dt * 100;
		if (moveSpeed > maxSpeed + plusMaxSpeed)
			moveSpeed -= dt * 500;
		if (abs(moveSpeed - maxSpeed + plusMaxSpeed) < dt * 200)
			moveSpeed = maxSpeed;

		plusMaxSpeed += dt * 6 / 7;

		if (pita(scene->player->GetCollisionPos(), GetCollisionPos()) > 1500)
		{
			if (timer > 5)
			{
				SetWalk(scene->player->GetCollisionPos());
				timer = 0;
			}
		}
		else if (timer > 0.5f)
		{
			SetWalk(scene->player->GetCollisionPos());
			timer = 0;
		}
	}
	
	if (currentTile->spr->color == D3DXCOLOR(1, 166 / (float)255, 142 / (float)255, 1))
	{
		maxSpeed = 100;
		for_iter(iter, animationMap)
		{
			((MultiDirAnimation*)iter->second)->GetYellow();
		}
	}
	else
		maxSpeed = 200;

	for_iter(obj, scene->objList)
	{
		if (*obj == this) continue;

		if (circleCollision(GetCollisionPos(), (*obj)->GetCollisionPos(), (*obj)->colRange) && !(*obj)->collisionAble)
		{
			if ((*obj)->type == GRASS_TRAP)
			{
				FallDown(*obj);
			}
			if ((*obj)->type == WATER_TRAP)
			{
				FallDown(*obj);
			}

			if ((*obj)->type == BOMB)
			{
				canMove = false;
				for_iter(iter, animationMap)
					((MultiDirAnimation*)(*iter).second)->Stop();
				moveSpeed = 0;

				((Building*)*obj)->onDead();

				for_iter(iter, animationMap)
				{
					Sprite * spr = (Sprite*)(iter->second);
					spr->color.r = 1;
					spr->color.b = 1;
				}

				changeColorTimer.reset(0.01f, 25);
				changeColorTimer.onTick = [=](){
					for_iter(iter, animationMap)
					{
						Sprite * spr = (Sprite*)(iter->second);
						spr->color.r -= 0.028f;
						spr->color.b -= 0.028f;
					}
				};

			}

			fallDownTimer.reset(5, 1);
			fallDownTimer.onFinished = [=]()
			{
				if (!isAttacked)
					canMove = true;

				for_iter(iter, animationMap)
					((MultiDirAnimation*)(*iter).second)->Play();

				for_iter(iter, animationMap)
				{
					Sprite * spr = (Sprite*)(iter->second);
					spr->color.r = 0.3f;
					spr->color.b = 0.3f;
				}

				changeColorTimer.reset(0.01f, 25);
				changeColorTimer.onTick = [=](){
					for_iter(iter, animationMap)
					{
						Sprite * spr = (Sprite*)(iter->second);
						spr->color.r += 0.028f;
						spr->color.b += 0.028f;
					}
				};

				moveSpeed = 50;
			};
		}
	}
}

void Follower::FallDown(Object *obj)
{
	if (obj)
		((Building*)obj)->onDead();

	canMove = false;
	ChangeAnimation(FALL_DOWN);
	asset.getSound(L"sound/Enemy/Falldown.wav")->Play();
	moveSpeed = 0;
	
	fallDownTimer.reset(2.3, 1);
	fallDownTimer.onFinished = [=]()
	{
		if (!isAttacked)
			canMove = true;
		moveSpeed = 50;
	};
}