#include "DXUT.h"
#include "EndScene.h"
#include "World.h"
#include "MainScene.h"
#include "Asset.h"

EndScene::EndScene(bool isClear) : isClear(isClear)
{
	if (isClear)
	{
		addChild(ed = new Animation("image/OutGame/Win/Back", 40, 14, false));
		addChild(spr = new Sprite("image/OutGame/Win/Front.png"));
	}
	else
	{
		addChild(ed = new Animation("image/OutGame/Lose/Back", 30, 12, false));
		addChild(spr = new Sprite("image/OutGame/Lose/Front.png"));
	}
	spr->color.a = 0;
	ed->stop = true;

	addChild(black = new Sprite("image/black.png"));
	black->color.a = 1;
}

EndScene::~EndScene()
{
}

void EndScene::update(float dt)
{
	if (isClear)
	{
		if (!asset.getSound(L"sound/ClearScene.wav")->IsSoundPlaying())
			asset.getSound(L"sound/ClearScene.wav")->Play();
	}
	else if (!asset.getSound(L"sound/BadScene.wav")->IsSoundPlaying())
			asset.getSound(L"sound/BadScene.wav")->Play();

	Scene::update(dt);

	black->color.a -= dt * 1.5f; 

	if (black->color.a <= 0 && ed->stop && world.getKeyState(VK_LBUTTON) == 1)
	{
		ed->stop = false;
	}
	if (ed->currentFrame >= ed->textures.size() - 1)
	{
		spr->color.a += dt;

		if (spr->color.a>= 1 && world.getKeyState(VK_LBUTTON) == 1)
		{
			asset.getSound(L"sound/ClearScene.wav")->Stop();
			asset.getSound(L"sound/BadScene.wav")->Stop();
			world.changeScene(new MainScene());
			return;
		}
	}
}