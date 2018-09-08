#include "DXUT.h"
#include "UI.h"
#include "GameScene.h"
#include "World.h"
#include "Asset.h"

UI::UI(Scene* scene) : gscene(scene), isOpenedInven(false), isOpenedCraftUI(false), howOpenedInven(0), howOpenedCraftUI(0), itemCount(), makedItemCount(), prograssTimer(0), isFarmingTimeFadeIn(true), isRunAwayFadeIn(true)
{
	makedItemCount[0] = 1;

	addChild(craftUI = new Sprite("image/makeUI/bg_inventory.png"));
	craftUI->pos = Vec2(-485, 84);

	addChild(inven = new Sprite("image/makeUI/bg_craft.png"));
	inven->pos = Vec2(891 + 315, 84);

	Sprite *prograssBarBack = new Sprite("image/ingameUI/bg_gauge.png");
	prograssBarBack->pos = Vec2(405, 10);
	addChild(prograssBarBack);

	Sprite *prograssBarUnderLine = new Sprite("image/ingameUI/bg_gauge_line.png");
	prograssBarUnderLine->pos = Vec2(405, 57);
	addChild(prograssBarUnderLine);

	addChild(prograssBarFarming = new Sprite("image/ingameUI/bg_gauge_bar_farming.png"));
	prograssBarFarming->pos = Vec2(407, 10);
	prograssBarFarming->visibleRect.right = 0;

	addChild(prograssBarHolding = new Sprite("image/ingameUI/bg_gauge_bar_holding.png"));
	prograssBarHolding->pos = Vec2(523, 10);
	prograssBarHolding->visibleRect.right = 0;

	Text *farmingT = new Text("파밍", 18, D3DXCOLOR(1, 1, 1, 1));
	addChild(farmingT);
	farmingT->pos = Vec2(452, 22);
	Text *holdingT = new Text("버티기", 18, D3DXCOLOR(1, 1, 1, 1));
	addChild(holdingT);
	holdingT->pos = Vec2(678, 22);

	for (int i = 0; i < 5; i++)
	{
		Text *text = new Text(to_string(i) + "분", 14, D3DXCOLOR(1, 1, 1, 1));
		addChild(text);
		text->pos = Vec2(394 + i * 117, 80);
	}

	for (int i = 0; i < 5; i++)
	{
		addChild(invenText[i] = new Text("0개 소지", 13, D3DXCOLOR(1, 1, 1, 1)));
		invenText[i]->pos = Vec2(1137, 218 + 92 * i);
		invenText[i]->visible = false;
	}

	Sprite *makedItem = new Sprite("image/ingameUI/bg_order_1~8.png");
	addChild(makedItem);
	makedItem->pos = Vec2(314, 616);

	for (int i = 0; i < 7; i++)
	{
		addChild(makedItemText[i] = new Text("0", 13, D3DXCOLOR(1, 1, 1, 1)));
		makedItemText[i]->pos = Vec2(464 + 81.5 * i, 640);
	}

	for (int i = 0; i < 8; i++)
	{
		addChild(selectToolSprite[i] = new Sprite("image/ingameUI/bg_order_" + to_string(i + 1) + ".png"));
		selectToolSprite[i]->pos = Vec2(314 + 81 * i, 616);
		selectToolSprite[i]->visible = false;
	}
	selectToolSprite[0]->visible = true;

	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 2; j++)
		{
			if (j == 0)
				addChild(canMake[i][j] = new Sprite("image/makeUI/ic_red.png"));
			else
				addChild(canMake[i][j] = new Sprite("image/makeUI/ic_green.png"));
			canMake[i][j]->pos = Vec2(400, 163 + i * 65);
			canMake[i][j]->visible = false;
		}
	}

	addChild(farmingTime = new Sprite("image/FarmingTime.png"));
	farmingTime->setCenter(Vec2(1280, 1000) / 2);
	farmingTime->color.a = 0;

	addChild(runAway = new Sprite("image/RanAway.png"));
	runAway->setCenter(Vec2(1280, 1000) / 2);
	runAway->color.a = 0;

	addChild(black = new Sprite("image/black.png"));
	black->color.a = 0;
}

UI::~UI()
{
}

bool UI::UseItem(ItemType type, bool use, int useCount)
{
	if (itemCount[type] >= useCount)
	{
		if (use)
			itemCount[type] -= useCount;
		return true;
	}

	return false;
}

void UI::MakeItem(Vec2 pos)
{
	GameScene *scene = (GameScene*)world.currentScene;

	if (makeItemTimer.active)
		return;

	makeItemTimer.reset(0.6f / makeItemList.size(), makeItemList.size());
	makeItemTimer.onTick = [=](){
		scene->itemList.push_back(makeItemList[makeItemTimer.count.first]);
		scene->addChild(makeItemList[makeItemTimer.count.first]);
		makeItemList[makeItemTimer.count.first]->setCenter(pos);
	};
	makeItemTimer.onFinished = [=](){ makeItemList.clear(); };
}

void UI::update(float dt)
{
	GameScene *scene = (GameScene*)world.currentScene;
	Entity::update(dt);

	if (isFarmingTimeFadeIn)
		farmingTime->color.a += dt * 0.7f;
	if (farmingTime->color.a >= 1)
		isFarmingTimeFadeIn = false;
	if (!isFarmingTimeFadeIn)
		farmingTime->color.a -= dt * 0.7f;

	if (isRunAwayFadeIn && prograssTimer > 60)
		runAway->color.a += dt * 0.7f;
	if (runAway->color.a >= 1)
		isRunAwayFadeIn = false;
	if (!isRunAwayFadeIn)
		runAway->color.a -= dt * 0.7f;

	prograssTimer += dt;

	if (prograssTimer >= 60 && prograssTimer <= 240)
		prograssBarHolding->visibleRect.right = prograssBarHolding->rect.right * (prograssTimer - 60) / 180;
	else if (prograssTimer < 60)
		prograssBarFarming->visibleRect.right = prograssBarFarming->rect.right * prograssTimer / 60;

	if (prograssTimer > 240)
		prograssTimer = 240;

	if (prograssTimer >= 240)
	{
		black->color.a += dt * 2;
		if (black->color.a >= 1)
		{
			scene->goToEnding = true;
		}
	}

	if (prograssTimer < 60 && !asset.getSound(L"sound/Ingame/Ingame_farming.wav")->IsSoundPlaying())
		asset.getSound(L"sound/Ingame/Ingame_farming.wav")->Play();
	else if (prograssTimer >= 60 && prograssTimer < 180 && !asset.getSound(L"sound/Ingame/Ingame_phase1.wav")->IsSoundPlaying())
	{
		asset.getSound(L"sound/Ingame/Ingame_farming.wav")->Stop();
		asset.getSound(L"sound/Ingame/Ingame_phase1.wav")->Play();
	}
	else if (prograssTimer >= 180 && !asset.getSound(L"sound/Ingame/Ingame_phase2.wav")->IsSoundPlaying())
	{
		asset.getSound(L"sound/Ingame/Ingame_phase1.wav")->Stop();
		asset.getSound(L"sound/Ingame/Ingame_phase2.wav")->Play();
	}
	
	if (world.getKeyState('T') == 1)
	{
		asset.getSound(L"sound/UI/UI_move.wav")->Reset();
		asset.getSound(L"sound/UI/UI_move.wav")->Play();
		if (!isOpenedCraftUI && howOpenedCraftUI == 0)
			isOpenedCraftUI = true;
		else if (isOpenedCraftUI && howOpenedCraftUI == 1)
			isOpenedCraftUI = false;
	}
	if (isOpenedCraftUI && howOpenedCraftUI < 1)
		craftUI->pos.x = lerp(-485, 0, howOpenedCraftUI = howOpenedCraftUI + dt * 3 > 1 ? 1 : howOpenedCraftUI + dt * 3);
	if (!isOpenedCraftUI && howOpenedCraftUI > 0)
		craftUI->pos.x = lerp(-485, 0, howOpenedCraftUI = howOpenedCraftUI - dt * 3 < 0 ? 0 : howOpenedCraftUI - dt * 3);

	if (isOpenedCraftUI && howOpenedCraftUI == 1)
	{
		if (itemCount[BRANCH_ITEM] > 0 && itemCount[LEAF_ITEM] > 0)
		{
			canMake[0][1]->visible = true;
			canMake[0][0]->visible = false;
		}
		else
		{
			canMake[0][1]->visible = false;
			canMake[0][0]->visible = true;
		}
		if (itemCount[BRANCH_ITEM] > 0 && itemCount[LEAF_ITEM] > 0 && itemCount[ROCK_ITEM] > 0)
		{
			canMake[1][0]->visible = false;
			canMake[1][1]->visible = true;
		}
		else
		{
			canMake[1][0]->visible = true;
			canMake[1][1]->visible = false;
		}
		if (itemCount[BOTTLE_ITEM] > 0 && scene->player->currentTile->code == WATER_TILE)
		{
			canMake[2][1]->visible = true;
			canMake[2][0]->visible = false;
		}
		else
		{
			canMake[2][1]->visible = false;
			canMake[2][0]->visible = true;
		}
		if (itemCount[BOTTLE_ITEM] > 0 && itemCount[LEAF_ITEM] > 0 && itemCount[ROCK_ITEM] > 0 && itemCount[SAP_ITEM] > 0)
		{
			canMake[3][1]->visible = true;
			canMake[3][0]->visible = false;
		}
		else
		{
			canMake[3][0]->visible = true;
			canMake[3][1]->visible = false;
		}

		if (itemCount[BRANCH_ITEM] > 1 && itemCount[LEAF_ITEM] > 1)
		{
			canMake[4][0]->visible = false;
			canMake[4][1]->visible = true;
		}
		else{
			canMake[4][0]->visible = true;
			canMake[4][1]->visible = false;
		}
		if (itemCount[BOTTLE_ITEM] > 1 && itemCount[SAP_ITEM] > 0)
		{
			canMake[5][1]->visible = true;
			canMake[5][0]->visible = false;
		}
		else
		{
			canMake[5][1]->visible = false;
			canMake[5][0]->visible = true;
		}
		if (itemCount[BRANCH_ITEM] > 0 && itemCount[SAP_ITEM] > 2 && itemCount[ROCK_ITEM] > 1)
		{
			canMake[6][1]->visible = true;
			canMake[6][0]->visible = false;
		}
		else
		{
			canMake[6][0]->visible = true;
			canMake[6][1]->visible = false;
		}
	}
	else
		for (int i = 0; i < 7; i++)
			for (int j = 0; j < 2; j++)
				canMake[i][j]->visible = false;

	if (world.getKeyState(VK_F1) == 1 && canMake[0][1]->visible && makedItemCount[GRASS_TRAP_TOOL] < 9)
	{
		asset.getSound(L"sound/UI/Create.wav")->Reset();
		asset.getSound(L"sound/UI/Create.wav")->Play();
		itemCount[BRANCH_ITEM]--;
		itemCount[LEAF_ITEM]--;
		makedItemCount[GRASS_TRAP_TOOL]++;
	}
	if (world.getKeyState(VK_F2) == 1 && canMake[1][1]->visible && makedItemCount[SLINGSHOT_TOOL] < 9)
	{
		asset.getSound(L"sound/UI/Create.wav")->Reset();
		asset.getSound(L"sound/UI/Create.wav")->Play();
		itemCount[BRANCH_ITEM]--;
		itemCount[LEAF_ITEM]--;
		itemCount[ROCK_ITEM]--;
		makedItemCount[SLINGSHOT_TOOL]++;
	}
	if (world.getKeyState(VK_F3) == 1 && canMake[2][1]->visible && makedItemCount[WATER_TOOL] < 9)
	{
		asset.getSound(L"sound/UI/Create.wav")->Reset();
		asset.getSound(L"sound/UI/Create.wav")->Play();
		itemCount[BOTTLE_ITEM]--;
		makedItemCount[WATER_TOOL]++;
	}
	if (world.getKeyState(VK_F4) == 1 && canMake[3][1]->visible && makedItemCount[BOMB_TOOL] < 9)
	{
		asset.getSound(L"sound/UI/Create.wav")->Reset();
		asset.getSound(L"sound/UI/Create.wav")->Play();
		itemCount[BOTTLE_ITEM]--;
		itemCount[SAP_ITEM]--;
		itemCount[LEAF_ITEM]--;
		itemCount[ROCK_ITEM]--;
		makedItemCount[BOMB_TOOL]++;
	}
	if (world.getKeyState(VK_F5) == 1 && canMake[4][1]->visible && makedItemCount[WATER_TRAP_TOOL] < 9)
	{
		asset.getSound(L"sound/UI/Create.wav")->Reset();
		asset.getSound(L"sound/UI/Create.wav")->Play();
		itemCount[BRANCH_ITEM] -= 2;
		itemCount[LEAF_ITEM] -= 2;
		makedItemCount[WATER_TRAP_TOOL]++;
	}
	if (world.getKeyState(VK_F6) == 1 && canMake[5][1]->visible && makedItemCount[BOTTLE_TRAP_TOOL] < 9)
	{
		asset.getSound(L"sound/UI/Create.wav")->Reset();
		asset.getSound(L"sound/UI/Create.wav")->Play();
		itemCount[BOTTLE_ITEM] -= 2;
		itemCount[SAP_ITEM]--;
		makedItemCount[BOTTLE_TRAP_TOOL]++;
	}
	if (world.getKeyState(VK_F7) == 1 && canMake[6][1]->visible && makedItemCount[TOUCH_TOOL] < 9)
	{
		asset.getSound(L"sound/UI/Create.wav")->Reset();
		asset.getSound(L"sound/UI/Create.wav")->Play();
		itemCount[BRANCH_ITEM]--;
		itemCount[SAP_ITEM] -= 3;
		itemCount[SAP_ITEM] -= 2;
		makedItemCount[TOUCH_TOOL]++;
	}

	if (world.getKeyState('I') == 1)
	{
		asset.getSound(L"sound/UI/UI_move.wav")->Reset();
		asset.getSound(L"sound/UI/UI_move.wav")->Play();
		if (!isOpenedInven && howOpenedInven == 0)
			isOpenedInven = true;
		else if (isOpenedInven && howOpenedInven == 1)
			isOpenedInven = false;
	}
	if (isOpenedInven && howOpenedInven < 1)
		inven->pos.x = lerp(891 + 315, 891, howOpenedInven = howOpenedInven + dt * 3 > 1 ? 1 : howOpenedInven + dt * 3);
	if (!isOpenedInven && howOpenedInven > 0)
		inven->pos.x = lerp(891 + 315, 891, howOpenedInven = howOpenedInven - dt * 3 < 0 ? 0 : howOpenedInven - dt * 3);

	if (isOpenedInven && howOpenedInven == 1)
		for (int i = 0; i < 5; i++)
			invenText[i]->visible = true;
	else
		for (int i = 0; i < 5; i++)
			invenText[i]->visible = false;

	for (int i = 0; i < 5; i++)
		invenText[i]->text = to_string(itemCount[i]) + "개 소지";

	for (int i = 0; i < 7; i++)
		makedItemText[i]->text = to_string(makedItemCount[i + 1]);

	for (int i = 0; i < 8; i++)
		if (selectToolSprite[i]->visible && makedItemCount[i] <= 0)
		{
			for (int j = 0; j < 8; j++)
				selectToolSprite[j]->visible = false;
			selectToolSprite[0]->visible = true;
		}

	for (int i = 0; i < 8; i++)
	{
		if (world.getKeyState('1' + i) == 1 && scene->player->canMove && makedItemCount[i] > 0)
		{
			asset.getSound(L"sound/itemSelecting.wav")->Reset();
			asset.getSound(L"sound/itemSelecting.wav")->Play();

			for (int j = 0; j < 8; j++)
				selectToolSprite[j]->visible = false;
			selectToolSprite[i]->visible = true;

			if (scene->waitBuild != nullptr)
			{
				removeChild(scene->waitBuild);
				scene->waitBuild = nullptr;
			}
		}
		if (selectToolSprite[i]->visible && !scene->waitBuild && scene->player->canMove)
		{
			if (i == GRASS_TRAP_TOOL)
				scene->addChild(scene->buildingFactory.MakeBuilding(GRASS_TRAP));
			if (i == BOMB_TOOL)
				scene->addChild(scene->buildingFactory.MakeBuilding(BOMB));
			if (i == WATER_TRAP_TOOL)
				scene->addChild(scene->buildingFactory.MakeBuilding(WATER_TRAP));
			if (i == TOUCH_TOOL)
				scene->addChild(scene->buildingFactory.MakeBuilding(TOUCH));
		}
	}
}
