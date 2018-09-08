#include "DXUT.h"
#include "MainScene.h"
#include "World.h"
#include "GameScene.h"
#include "Asset.h"

MainScene::MainScene() : howToCount(-1), moveTitle(0)
{
	Sprite *back = new Sprite("image/Outgame/Main/Main.png");
	addChild(back);

	title = new Sprite("image/Outgame/Main/Logo.png");
	addChild(title);
	title->pos = Vec2(321, 0);

	addChild(gameStartB = new Sprite("image/Outgame/Main/GameStart/1.png"));
	gameStartB->pos = Vec2(493, 580);
	addChild(howToB = new Sprite("image/Outgame/Main/HowToPlay/1.png"));
	howToB->pos = Vec2(734, 580);
	addChild(exitB = new Sprite("image/Outgame/Main/Exit/1.png"));
	exitB->pos = Vec2(973, 580);

	gameStartB->scaleCenter = gameStartB->rect.center();
	howToB->scaleCenter = howToB	->rect.center();
	exitB->scaleCenter = exitB	    ->rect.center();

	for (int i = 0; i < 5; i++)
	{
		addChild(howToEx[i] = new Sprite("image/Outgame/Main/HowTo/" + to_string(i) + ".png"));
		howToEx[i]->visible = false;
	}

	addChild(cursor = new Sprite("image/ui/mouse/walk.png"));
}

MainScene::~MainScene()
{
}

void MainScene::update(float dt)
{
	if (!asset.getSound(L"sound/MainScene.wav")->IsSoundPlaying())
		asset.getSound(L"sound/MainScene.wav")->Play();

	Scene::update(dt);
	cursor->setCenter(world.getMousePos());
	//title->pos.y = 10 + sin(moveTitle) * 10;
	//moveTitle += dt;
	if (howToCount >= 0)
	{
		if (world.getKeyState(VK_LBUTTON) == 1)
		{
			asset.getSound(L"sound/UI/ButtonClick.wav")->Stop();
			asset.getSound(L"sound/UI/ButtonClick.wav")->Play();
			if (howToCount < 4)
			{
				howToEx[howToCount]->visible = false;
				howToEx[++howToCount]->visible = true;
			}
			else
			{
				howToEx[4]->visible = false;
				howToCount = -1;
			}
			return;
		}
	}
	else
	{
		if (gameStartB->rectWithPos().intersects(world.getMousePos()))
		{
			gameStartB->scale = Vec2(1.2f, 1.2f);
			if (world.getKeyState(VK_LBUTTON) == 1)
			{
				asset.getSound(L"sound/UI/ButtonClick.wav")->Reset();
				asset.getSound(L"sound/UI/ButtonClick.wav")->Play();
				asset.getSound(L"sound/MainScene.wav")->Stop();
				world.changeScene(new GameScene());
				return;
			}
		}
		else
			gameStartB->scale = Vec2(1, 1);

		if (howToB->rectWithPos().intersects(world.getMousePos()))
		{
			howToB->scale = Vec2(1.2f, 1.2f);
			asset.getSound(L"sound/UI/ButtonClick.wav")->Reset();
			asset.getSound(L"sound/UI/ButtonClick.wav")->Play();
			if (world.getKeyState(VK_LBUTTON) == 1)
			{
				howToCount = 0;
				howToEx[howToCount]->visible = true;
				return;
			}
		}
		else
			howToB->scale = Vec2(1, 1);

		if (exitB->rectWithPos().intersects(world.getMousePos()))
		{
			exitB->scale = Vec2(1.2f, 1.2f);
			if (world.getKeyState(VK_LBUTTON) == 1)
			{
				asset.getSound(L"sound/UI/ButtonClick.wav")->Reset();
				asset.getSound(L"sound/UI/ButtonClick.wav")->Play();
				exit(0);
				return;
			}
		}
		else
			exitB->scale = Vec2(1, 1);
	}
}

