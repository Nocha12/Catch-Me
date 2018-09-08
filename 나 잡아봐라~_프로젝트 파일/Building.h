#pragma once
#include "Object.h"
#include "Animation.h"
#include "Timer.h"

class Building : public Object
{
public:
	Building();
	~Building();

	void update(float dt);

	Tile *buildTile;
	bool buildMode;
	bool canBuild;
	float buildingTimer;
	float buildingDelay;
	float dustTimer;
	float fade;
	Timer fadeOutTimer;
	Timer scaleTimer;

	list<Sprite*> checkList;
	Sprite *sprite;
	Sprite *animation;

	void instantBuild();
	void instantBuild(bool setBuildTile, bool isMove);
	void onDead();
	void setCheckTile();
	void Build(float dt);
	void CheckCanBuild();
	void CheckCanBuild(TILE_CODE code);
	void FadeOut();
	void ScaleUp();
};

