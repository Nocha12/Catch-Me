#include "DXUT.h"
#include "LoadingScene.h"
#include "Asset.h"
#include "MainScene.h"
#include "World.h"

LoadingScene::LoadingScene()
{
	Animation *back;
	addChild(back = new Animation("image/OutGame/Loading/Back", 35, 15, false, false));

	Sprite *sprite = new Sprite("image/OutGame/Loading/BarStroke.png");
	addChild(sprite);
	
	addChild(bar = new Sprite("image/OutGame/Loading/Bar.png"));
	bar->pos = Vec2(339, 595);

	asset.ready("image");
}

LoadingScene::~LoadingScene()
{
}

void LoadingScene::update(float dt)
{
	Scene::update(dt);

	for (int i = 0; i < 10; i++)
	{
		asset.loadNext();
		bar->visibleRect.right = bar->rect.right * asset.filesLoaded / asset.filesToLoad;

		if (asset.filesLoaded == asset.filesToLoad)
		{
			world.changeScene(new MainScene());

			return;
		}
	}
}


