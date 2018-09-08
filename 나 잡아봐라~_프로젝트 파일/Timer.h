#pragma once
#include "Define.h"
class Timer
{
public:
	Timer();
	~Timer(void);

	void update(float dt);
	
	void start();
	void stop();

	void reset(float duration = 0, int count = 0);

	function<void()> onTick;
	function<void()> onFinished;

	pair<float, float> duration;
	pair<int, int> count;

	bool active;
};

