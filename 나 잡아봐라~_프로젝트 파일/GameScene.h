#pragma once
#include "Scene.h"
#include "Tile.h"
#include "StageMap.h"
#include "UI.h"
#include "Player.h"
#include "Cursor.h"
#include "Building.h"
#include "BuildingFactory.h"
#include "Sprite.h"
#include "Particle.h"
#include "Arrow.h"
#include "Item.h"
#include "AfterImage.h"

class GameScene :
	public Scene
{
public:
	GameScene();
	~GameScene();

	void render();
	void update(float dt);

	UI* ui;

	StageMap* stageMap;
	Tile* mapTile[TILECOUNT][TILECOUNT];
	Tile* GetTile(Vec2 pos);
	Tile* GetRandomTile(TILE_CODE tileCode);
	Tile* GetRandomSideTile(TILE_CODE tileCode);
	Tile* GetRandomCenterTile(TILE_CODE tileCode);

	Tile *startTile;

	list<Object*> objList;
	list<Sprite*> dustList;
	list<Particle*> particleList;
	list<Arrow*> arrowList;
	list<Item*> itemList;
	list<AfterImage*> afterImageList;

	Cursor *cursor;
	Player *player;
	Building *raft;
	Building *waitBuild;
	BuildingFactory buildingFactory;

	bool goToEnding;
	
	bool isInit;
	float followerRespawnTimer;

	Entity *followerT;
	float followrDelayTimer;

	map<CursorType, Sprite*> cursorImg;

	void ChangeCursor(CursorType type);
};

