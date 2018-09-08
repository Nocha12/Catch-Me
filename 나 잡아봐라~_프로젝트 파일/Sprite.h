#pragma once
#include "Define.h"
#include "Entity.h"
#include "Texture.h"

class Sprite :
	public Entity
{
public:
	Sprite();
	Sprite(path path);
	virtual ~Sprite(void);
	void render();
	void setPath(path path);
	void setTexture(Texture *texture);

	Texture* texture;
	D3DXCOLOR color;
	bool blending;
};
