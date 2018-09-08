#pragma once
#include "Entity.h"
#include "Animation.h"
#include "Unit.h"
#include "Timer.h"

class MultiDirAnimation : public Sprite
{
public:
	MultiDirAnimation(Object *target, string path, int fileNum, float fps = 10, bool isLoop = true, bool isPng = false);
	~MultiDirAnimation();

	void update(float dt);

	void Reset();
	void Stop();
	void Play();
	void FaidIn();
	void FaidOut();
	void Invisible();
	void GetRed();
	void GetYellow();
	void GetBlack();

	Object *target;
	Animation *animation[8];

	Timer faidTimer;
	Timer redTimer;
	Timer blackTimer;
	Timer yellowTimer;

	bool isFin;
	float fps;
	float currentFrame;
	int direction;
	int size;
	float faid;
	bool isPlaying;
};

