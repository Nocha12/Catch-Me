#pragma once
#include "Entity.h"
#include "Sprite.h"
class Animation :
	public Sprite
{
public:
	Animation(const string& path, int frameNum, float fps = 10, bool loop = true, bool isPng = false);
	~Animation(void);
	void update(float dt);

	vector<Texture*> textures;
	float currentFrame;
	float fps;
	bool loop;
	bool stop;
	bool isFin;
};

