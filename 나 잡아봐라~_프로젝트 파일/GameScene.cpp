#include "DXUT.h"
#include "GameScene.h"
#include "World.h"
#include "Player.h"
#include "MainScene.h"
#include "EndScene.h"
#include "Follower.h"
#include "Asset.h"

GameScene::GameScene() : buildingFactory(this), waitBuild(nullptr), isInit(false), followerT(nullptr), followerRespawnTimer(0), followrDelayTimer(0), goToEnding(false)
{
	ui = new UI(this);

	for (int i = 0; i < TILECOUNT; i++){
		for (int j = 0; j < TILECOUNT; j++){
			mapTile[i][j] = nullptr;
		}
	}
	stageMap = new StageMap(this);
	addChild(stageMap);

	player = new Player();
	player->SetCollisionPos(GetRandomCenterTile(GROUND_TILE)->GetCenter());
	addChild(player);

	follow(player);
	pos = -player->center() + offset;
	player->movePos = player->GetCollisionPos();
	objList.push_back(player);
	
	startTile = GetTile(player->GetCollisionPos());
	startTile->spr->color = D3DXCOLOR(194.0f / 256, 170.0f / 256, 149.0f / 256, 1);

	if (-pos.x < 0) pos.x = 0;
	if (-pos.y < 20) pos.y = -20;
	if (-pos.x > MAPWIDTH - 1280 - TILEWIDTH / 2) pos.x = -(MAPWIDTH - 1280 - TILEWIDTH / 2);
	if (-pos.y > MAPHEIGHT / 2 - 720 - TILEHEIGHT / 2) pos.y = -(MAPHEIGHT / 2 - 720 - TILEHEIGHT / 2);

	addChild(cursor = new Cursor());
	addChild(cursorImg[ATTACK_CURSOR] = new Sprite("image/ui/mouse/attack.png"));
	addChild(cursorImg[HAMMER_CURSOR] = new Sprite("image/ui/mouse/hammer.png"));
	addChild(cursorImg[PICK_CURSOR] = new Sprite("image/ui/mouse/rock.png"));
	addChild(cursorImg[SLEEP_CURSOR] = new Sprite("image/ui/mouse/sleep.png"));
	addChild(cursorImg[AX_CURSOR] = new Sprite("image/ui/mouse/tree.png"));
	addChild(cursorImg[X_CURSOR] = new Sprite("image/ui/mouse/x.png"));
	addChild(cursorImg[MOVE_CURSOR] = new Sprite("image/ui/mouse/walk.png"));
	ChangeCursor(MOVE_CURSOR);
}

GameScene::~GameScene()
{
	SAFE_DELETE(ui);
}

void GameScene::render()
{
	Scene::render();

	for_iter(child, stageMap->children)
	{
		bool flag = true;
		if (-pos.x - 300 > (*child)->pos.x) flag = false;
		if (-pos.y - 300 > (*child)->pos.y) flag = false;
		if (-pos.x + 1280 < (*child)->pos.x) flag = false;
		if (-pos.y + 720  < (*child)->pos.y) flag = false;

		if (flag)
			(*child)->render();
	}

	for_iter(iter, afterImageList)
		(*iter)->render();

	for_iter(iter, arrowList)
		(*iter)->render();
	
	objList.sort([](Object *o1, Object *o2){
		return o1->GetCollisionPos().y < o2->GetCollisionPos().y ? true : false;
	});
	for_iter(iter, objList)
		(*iter)->render();

	if (waitBuild)
		waitBuild->render();

	for_iter(iter, dustList)
		(*iter)->render();
	for_iter(iter, particleList)
		(*iter)->render();

	ui->render();

	for_iter(iter, itemList)
		(*iter)->render();

	for_iter(iter, cursorImg)
		(*iter).second->render();
}

void GameScene::update(float dt)
{
	Scene::update(dt);
	ui->update(dt);
	
	if (goToEnding)
	{
		asset.getSound(L"sound/Ingame/Ingame_farming.wav")->Stop();
		asset.getSound(L"sound/Ingame/Ingame_phase1.wav")->Stop();
		asset.getSound(L"sound/Ingame/Ingame_phase2.wav")->Stop();

		if (ui->prograssTimer >= 240)
			world.changeScene(new EndScene(true));
		else
			world.changeScene(new EndScene(false));
		return;
	}

	for_iter(iter, cursorImg)
		(*iter).second->setCenter(world.getMousePos() - pos);

	followerRespawnTimer += dt;
	
	if (followerRespawnTimer > 60 && !followerT)
	{
		for (int i = 0; i < 2; i++)
		{
			Follower *f = new Follower();
			f->SetCollisionPos(startTile->GetCenter());
			objList.push_back(f);
			addChild(f);
			cursor->canUseCursor = false;
			followerT = f;
			follow(f);
		}
	}
	
	if (followerT && pita(followerT->center() - offset, -pos) < 100)
	{
		if (!asset.getSound(L"sound/Siren.wav")->IsSoundPlaying())
			asset.getSound(L"sound/Siren.wav")->Play();
		followrDelayTimer += dt;
	}
	
	if (followrDelayTimer > 3)
	{
		asset.getSound(L"sound/Siren.wav")->Stop();
		cursor->canUseCursor = true;
		follow(player);
	}

	if (!isInit)
	{
		for (int i = 0; i < 35; i++)
			addChild(buildingFactory.MakeBuilding(ROCK, true, false, GROUND_TILE));
		for (int i = 0; i < 35; i++)
			addChild(buildingFactory.MakeBuilding(TREE, true, false, GROUND_TILE));
		for (int i = 0; i < 35; i++)
			addChild(buildingFactory.MakeBuilding(TRASH, true, false, GROUND_TILE));

		isInit = true;
	}
	if (world.getKeyState('0') == 1)
	{
		for (int i = 0; i < 15; i++)
		{
			Item *item1 = new Item(ROCK_ITEM);
			ui->makeItemList.push_back(item1);
		} for (int i = 0; i < 15; i++)
		{
			Item *item3 = new Item(LEAF_ITEM);
			ui->makeItemList.push_back(item3);
		} for (int i = 0; i < 15; i++)
		{
			Item *item4 = new Item(BRANCH_ITEM);
			ui->makeItemList.push_back(item4);
		} for (int i = 0; i < 15; i++)
		{
			Item *item5 = new Item(SAP_ITEM);
			ui->makeItemList.push_back(item5);
		} for (int i = 0; i < 15; i++)
		{
			Item *item5 = new Item(BOTTLE_ITEM);
			ui->makeItemList.push_back(item5);
		}

		ui->MakeItem(player->GetCollisionPos());
	}
	
	if (world.getKeyState(VK_OEM_MINUS) == 1)
		for (int i = 0; i < 8; i++)
			ui->makedItemCount[i] = 9;

	if (world.getKeyState(VK_OEM_PLUS) == 2)
	{
		followerRespawnTimer += dt * 10;
		ui->prograssTimer += dt * 10;
	}

	if (world.getKeyState(VK_SPACE) == 1)
	{
		follow(player);
	}

	if (world.getMousePos().x <= 10) pos.x += 800 * dt;
	if (world.getMousePos().y <= 10) pos.y += 800 * dt;
	if (world.getMousePos().x >= 1270) pos.x -= 800 * dt;
	if (world.getMousePos().y >= 710) pos.y -= 800 * dt;

	if (-pos.x < 0) pos.x = 0;
	if (-pos.y < 20) pos.y = -20;
	if (-pos.x > MAPWIDTH - 1280 - TILEWIDTH / 2) pos.x = -(MAPWIDTH - 1280 - TILEWIDTH / 2);
	if (-pos.y > MAPHEIGHT / 2 - 720 - TILEHEIGHT / 2) pos.y = -(MAPHEIGHT / 2 - 720 - TILEHEIGHT / 2);

	for_iter(dust, dustList)
	{
		(*dust)->scale += Vec2(dt, dt);
		(*dust)->color.a += dt;
		(*dust)->rotation += dt * 3;

		if ((*dust)->color.a >= 1)
		{
			removeChild(*dust);
			dust = dustList.erase(dust);
			if (dust == dustList.end())
				break;
		}
	}
} 

Tile* GameScene::GetTile(Vec2 pos)
{
	Vec2 p = Vec2((int)(pos.x + TILEWIDTH / 2) / TILEWIDTH, (int)(pos.y + TILEHEIGHT / 2) / TILEHEIGHT * 2);
	Vec2 t = Vec2((int)(pos.x + TILEWIDTH / 2) % TILEWIDTH, (int)(pos.y + TILEHEIGHT / 2) % TILEHEIGHT);

	float r = (TILEHEIGHT * 0.25f) / (TILEWIDTH * 0.25f);
	if (t.y < -r * t.x + TILEHEIGHT * 0.5f){
		p.x -= 1;
		p.y -= 1;
	}
	else if (t.y < r * t.x - TILEHEIGHT * 0.5f){
		p.y -= 1;
	}
	else if (t.y > r * t.x + TILEHEIGHT * 0.5f){
		p.x -= 1;
		p.y += 1;
	}
	else if (t.y > -r * t.x + TILEHEIGHT * 1.5f){
		p.y += 1;
	}

	p.x = min(max(p.x, 0), TILECOUNT - 1);
	p.y = min(max(p.y, 0), TILECOUNT - 1);
	return mapTile[int(p.x)][int(p.y)];
}

Tile* GameScene::GetRandomTile(TILE_CODE tileCode)
{
	vector<Tile*> tiles;
	for (int i = 0; i < TILECOUNT; i++){
		for (int j = 0; j < TILECOUNT; j++){
			if (mapTile[i][j]->code == tileCode){
				tiles.push_back(mapTile[i][j]);
			}
		}
	}
	return tiles[random(0, tiles.size() - 1)];
}

Tile* GameScene::GetRandomSideTile(TILE_CODE tileCode)
{
	vector<Tile*> tiles;
	for (int i = 0; i < TILECOUNT; i++){
		for (int j = 0; j < TILECOUNT; j++){
			if (mapTile[i][j]->code == tileCode && (i < 5 || i > TILECOUNT - 5 || j < 5 || j > TILECOUNT - 5)){
				tiles.push_back(mapTile[i][j]);
			}
		}
	}
	return tiles[random(0, tiles.size() - 1)];
}

Tile* GameScene::GetRandomCenterTile(TILE_CODE tileCode)
{
	vector<Tile*> tiles;
	for (int i = 0; i < TILECOUNT; i++){
		for (int j = 0; j < TILECOUNT; j++){
			if (mapTile[i][j]->code == tileCode && (i > TILECOUNT / 2 - 10 && i < TILECOUNT / 2 + 10 && j > TILECOUNT / 2 - 10 && j < TILECOUNT / 2 + 10) && mapTile[i][j]->canBuild){
				tiles.push_back(mapTile[i][j]);
			}
		}
	}
	return tiles[random(0, tiles.size() - 1)];
}

void GameScene::ChangeCursor(CursorType type)
{
	for_iter(iter, cursorImg)
		(*iter).second->visible = false;
	cursorImg[type]->visible = true;
}