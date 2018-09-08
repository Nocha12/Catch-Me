 #include "DXUT.h"
#include "StageMap.h"
#include "GameScene.h"

StageMap::StageMap(Scene* scene) : scene(scene)
{
	renderChildrenEnabled = false;
	init();
}

StageMap::~StageMap()
{
}

void StageMap::init()
{
	GameScene* GS = (GameScene*)scene;

	for (int i = 0; i < TILECOUNT; i++)
	for (int j = 0; j < TILECOUNT; j++)
	if (i < 3 || i > TILECOUNT - 3 || j < 3 || j > TILECOUNT - 3)
		tiles[i][j] = WATER_TILE;
	else{
		if (random(0, 2) == 0){
			tiles[i][j] = WATER_TILE;
		}
		else{
			tiles[i][j] = GROUND_TILE;
		}
	}
		

	for (int i = 0; i < 10; i++){
		mapCrop(WATER_TILE);
		mapCrop(GROUND_TILE);
	}

	for (int q = 0; q < 5; q++){
		for (int i = 0; i < TILECOUNT; i++){
			for (int j = 0; j < TILECOUNT; j++){
				if (tiles[i][j] == GROUND_TILE){
					if (random(0, 2) == 0){
						tiles[i][j] = SAND_TILE;
					}
				}
			}
		}

		for (int i = 0; i < 10; i++){
			mapCrop(GROUND_TILE);
			mapCrop(SAND_TILE);
		}
	}
	
	for (int i = 0; i < TILECOUNT; i++){
		for (int j = 0; j < TILECOUNT; j++){
			if (i < 3 || i > TILECOUNT - 3 || j < 3 || j > TILECOUNT - 3){
				tiles[i][j] = DEEP_WATER_TILE;
			}
		}
	}

	for (int i = 0; i < TILECOUNT; i++){
		for (int j = 0; j < TILECOUNT; j++){
			GS->mapTile[i][j] = new Tile(tiles[i][j]); 
			GS->mapTile[i][j]->setCenter(Vec2(i * TILEWIDTH, j * TILEHEIGHT / 2 + 32));
			//GS->mapTile[i][j]->setCenter(Vec2(i * TILEWIDTH, j * TILEHEIGHT - 17 - TILEHEIGHT / 2 * (j - 1)));
			if (j % 2 == 1){
				GS->mapTile[i][j]->pos.x += TILEWIDTH / 2;
			}
			GS->mapTile[i][j]->tilePos = Vec2(i, j);
			GS->mapTile[i][j]->count += i * 0.5f;
			addChild(GS->mapTile[i][j]);
		}
	}

	children.sort([](Entity* e1, Entity* e2){
		return e1->pos.y < e2->pos.y;
	});
}

void StageMap::mapCrop(int tileCode)
{
	for (int i = 0; i < TILECOUNT; i++){
		for (int j = 0; j < TILECOUNT; j++){
			int cnt = 0;
			if (tiles[i - 1][j - 1] == tileCode)	cnt++;
			if (tiles[i][j - 1] == tileCode)		cnt++;
			if (tiles[i + 1][j - 1] == tileCode)	cnt++;

			if (tiles[i - 1][j] == tileCode)		cnt++;
			if (tiles[i][j] == tileCode)			cnt++;
			if (tiles[i + 1][j] == tileCode)		cnt++;

			if (tiles[i - 1][j + 1] == tileCode)	cnt++;
			if (tiles[i][j + 1] == tileCode)		cnt++;
			if (tiles[i + 1][j + 1] == tileCode)	cnt++;

			if (cnt >= 5)
				tiles[i][j] = tileCode;
		}
	}
}