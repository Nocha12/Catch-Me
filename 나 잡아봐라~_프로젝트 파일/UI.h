#pragma once
#include "Entity.h"
#include "Scene.h"
#include "Sprite.h"
#include "Item.h"
#include "Timer.h"
#include "Text.h"

class UI :
	public Entity
{
public:
	UI(Scene* scene);
	~UI();

	void update(float dt);

	Scene* gscene;

	Sprite *mapUI[TILECOUNT][TILECOUNT];

	vector<Item*> makeItemList;

	void MakeItem(Vec2 pos);
	bool UseItem(ItemType type, bool use = false, int useCount = 0);

	bool isOpenedInven;
	bool isOpenedCraftUI;
	float howOpenedInven;
	float howOpenedCraftUI;

	Timer makeItemTimer;

	Sprite *craftUI;
	Sprite *inven;

	Sprite *canMake[7][2];

	Text *invenText[5];
	Text *makedItemText[7];
	int itemCount[5];
	int makedItemCount[8];

	Sprite *prograssBarFarming;
	Sprite *prograssBarHolding;
	float prograssTimer;

	Sprite *farmingTime;
	Sprite *runAway;

	bool isFarmingTimeFadeIn;
	bool isRunAwayFadeIn;

	Sprite *selectToolSprite[8];
	Sprite *black;
};

