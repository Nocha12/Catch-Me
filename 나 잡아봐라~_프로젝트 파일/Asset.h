#pragma once
#include "Define.h"
#include "Texture.h"

class Asset
{
public:
	Asset(void);
	~Asset(void);
	void init();
	void dispose();
	void ready(string rootFolderPath);
	void loadNext();
	Texture* getTexture(path p);
	CSound* getSound(wpath p);
	LPD3DXEFFECT getShader(string path);
	
	map<path, Texture*> textures;
	map<wpath, CSound*> sounds;
	list<LPD3DXEFFECT> shaders;
	CSoundManager _s;

	list<path> paths;
	list<path>::iterator pathIter;

	int filesLoaded;
	int filesToLoad;
};

extern Asset asset;