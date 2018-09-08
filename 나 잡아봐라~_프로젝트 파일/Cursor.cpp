#include "DXUT.h"
#include "Cursor.h"
#include "GameScene.h"
#include "World.h"
#include "Unit.h"

Cursor::Cursor() : canUseCursor(true)
{
}

Cursor::~Cursor()
{
}

void Cursor::update(float dt)
{
	Entity::update(dt);
	GameScene *scene = (GameScene*)world.currentScene;

	if (canUseCursor)
	{
		if (world.getKeyState(VK_LBUTTON) == 1)
		{
			if (scene->waitBuild != nullptr && scene->waitBuild->canBuild)
			{
				if (scene->ui->selectToolSprite[GRASS_TRAP_TOOL]->visible)
					scene->ui->makedItemCount[GRASS_TRAP_TOOL]--;
				if (scene->ui->selectToolSprite[TOUCH_TOOL]->visible)
					scene->ui->makedItemCount[TOUCH_TOOL]--;
				if (scene->ui->selectToolSprite[WATER_TRAP_TOOL]->visible)
					scene->ui->makedItemCount[WATER_TRAP_TOOL]--;
			
				scene->waitBuild->buildMode = false;
				scene->objList.push_back(scene->waitBuild);
				if (scene->waitBuild->type == BOMB)
					scene->waitBuild->instantBuild(true, false);
				else
					scene->waitBuild->instantBuild(true, true);
				scene->waitBuild->collisionAble = false;
				scene->waitBuild = nullptr;
			}
		}

		if (!scene->player->canMove)
			scene->ChangeCursor(X_CURSOR);
		else if (scene->waitBuild)
			scene->ChangeCursor(HAMMER_CURSOR);

		else
		{
			bool check = true;
			for_iter(iter, scene->objList)
			{
				if (collision((*iter)->pos, world.getMousePos() - scene->pos, (*iter)->rect) && (*iter)->type != PLAYER)
				{
					if ((*iter)->type == ROCK)
						scene->ChangeCursor(PICK_CURSOR);
					else if ((*iter)->type == ROCK)
						scene->ChangeCursor(X_CURSOR);
					else if ((*iter)->type == TREE)
						scene->ChangeCursor(AX_CURSOR);
				
					check = false;
				}
			}
			if (scene->ui->selectToolSprite[SLINGSHOT_TOOL]->visible || scene->ui->selectToolSprite[BOTTLE_TRAP_TOOL]->visible)
				scene->ChangeCursor(ATTACK_CURSOR);
			else if (check)
				scene->ChangeCursor(MOVE_CURSOR);
		}

		if (world.getKeyState(VK_RBUTTON) == 1 && scene->player->canMove)
		{
			Object *target = nullptr;

			scene->player->attackTarget = nullptr;

			for_iter(iter, scene->objList)
			{
				if (collision((*iter)->pos, world.getMousePos() - scene->pos, (*iter)->rect) && (*iter)->type != PLAYER)
				{
					target = *iter;
				}
			}
			if (target && !scene->waitBuild)
			{
				scene->player->routeList.clear();
				scene->player->attackTarget = target;
			
				if (target->type != BOTTLE_TRAP && !target->isUnit && ((Building*)target)->sprite->color.a == 1 && scene->ui->selectToolSprite[NONE_TOOL]->visible)
				{
					scene->player->SetWalk(world.getMousePos() - scene->pos);
					if (target->type == ROCK)
					{
						scene->player->canMove = false;
						scene->player->isPicking = true;
						scene->player->SetWalk(target->currentTile->GetCenter());
					}
					else if (target->type == TREE)
					{
						scene->player->canMove = false;
						scene->player->isCuttingTree = true;
						scene->player->SetWalk(target->currentTile->GetCenter());
					}
					else if (target->type == TRASH)
					{
						scene->player->canMove = false;
						scene->player->pickUpTrash = true;
						scene->player->SetWalk(target->currentTile->GetCenter());
					}
				}
			}
			else
				scene->player->SetWalk(world.getMousePos() - scene->pos);
		}
	}
	else 
		scene->ChangeCursor(X_CURSOR);
}
