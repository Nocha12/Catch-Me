#include "DXUT.h"
#include "Animation.h"
#include "Asset.h"

Animation::Animation(const string& path, int frameNum, float fps, bool loop, bool isPng)
:loop(loop), currentFrame(0), fps(fps), stop(false), isFin(false)
{
	for (int i = 0; i <= frameNum; i++)
	{
		if (!isPng)
		{
			Texture* t = asset.getTexture(path + "/" + to_string(i) + ".dds");
			textures.push_back(t);
		}
		else
		{
			Texture* t = asset.getTexture(path + "/" + to_string(i) + ".png");
			textures.push_back(t);
		}
	}
	texture = textures[0];

	visibleRect = rect = Rect(0, 0, texture->info.Width, texture->info.Height);
}

Animation::~Animation(void)
{
}

void Animation::update(float dt)
{
	if (stop) return;

	Sprite::update(dt);

	currentFrame += dt * fps;

	if ((int)currentFrame >= textures.size())
	{
		if (loop)
			currentFrame = 0;
		else
		{
			currentFrame = textures.size() - 1;
			isFin = true;
		}
	}

	texture = textures[int(currentFrame)];
}