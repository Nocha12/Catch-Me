#include "DXUT.h"
#include "MultiDirAnimation.h"

MultiDirAnimation::MultiDirAnimation(Object *target, string path, int fileNum, float fps, bool isLoop, bool isPng) : target(target), isPlaying(true), isFin(false), fps(fps)
{
	for (int i = 0; i < 8; ++i)
	{
		animation[i] = new Animation(path + "/" + to_string(i), fileNum, fps, isLoop, isPng);
		parent = animation[i];
	}
	visibleRect = rect = animation[0]->rect;
	size = animation[0]->textures.size() - 1;
}

MultiDirAnimation::~MultiDirAnimation()
{
	for (int i = 0; i < 8; i++)
	{
		SAFE_DELETE(animation[i]);
	}
}

void MultiDirAnimation::update(float dt)
{
	Sprite::update(dt);

	for (int i = 0; i < 8; i++)
	{
		animation[i]->fps = fps;
		animation[i]->update(dt);
		animation[i]->visible = false;
	}

	float angle = D3DXToDegree(target->moveRotation) - 45 - 22;
	while (angle < 0) angle += 360;
	int dir = angle / 45;

	animation[dir]->visible = true;
	texture = animation[dir]->texture;
	isFin = animation[dir]->isFin;
	direction = dir;
}

void MultiDirAnimation::Reset()
{
	for (int i = 0; i < 8; ++i)
	{
		animation[i]->currentFrame = 0;
	}
}

void MultiDirAnimation::Stop()
{
	for (int i = 0; i < 8; ++i)
	{
		animation[i]->stop = true;
	}
}

void MultiDirAnimation::Play()
{
	isPlaying = true;

	for (int i = 0; i < 8; ++i)
	{
		animation[i]->stop = false;
	}
}

void MultiDirAnimation::Invisible()
{
	for (int i = 0; i < 8; ++i)
		animation[i]->color = D3DXCOLOR(1, 1, 1, 0);
}

void MultiDirAnimation::FaidIn()
{
	faidTimer.reset(0.01f, 50);
	faid = 0;

	faidTimer.onTick = [=]()
	{
		faid += 0.02f;

		for (int i = 0; i < 8; ++i)
			animation[i]->color = D3DXCOLOR(1, 1, 1, faid);
	};
}

void MultiDirAnimation::FaidOut()
{
	faidTimer.reset(0.01f, 25);
	faid = 1;

	faidTimer.onTick = [=]()
	{
		faid -= 0.04f;

		for (int i = 0; i < 8; ++i)
			animation[i]->color = D3DXCOLOR(1, 1, 1, faid);
	};
}

void MultiDirAnimation::GetRed()
{
	color.b = 0.3f;
	color.g = 0.3f;
	redTimer.reset(0.01f, 25);
	redTimer.onTick = [=](){
		color.b += 0.028f;
		color.g += 0.028f;
	};
}

void MultiDirAnimation::GetYellow()
{
	color.r = 240 / (float)255;
	color.b = 0;
	yellowTimer.reset(0.01f, 25);
	yellowTimer.onTick = [=](){
		color.r += 240 / (float)255 / 25;
		color.b += 1 / (float)25;
	};
}


void MultiDirAnimation::GetBlack()
{
	for (int i = 0; i < 8; ++i)
	{
		animation[i]->color.r = 0.3f;
		animation[i]->color.b = 0.3f;
		animation[i]->color.g = 0.3f;
	}
	blackTimer.reset(0.01f, 25);
	blackTimer.onTick = [=](){
		for (int i = 0; i < 8; ++i)
		{
			animation[i]->color.r += 0.028f;
			animation[i]->color.b += 0.028f;
			animation[i]->color.g += 0.028f;
		}
	};
}
