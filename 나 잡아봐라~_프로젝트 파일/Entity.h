#pragma once
#include "Define.h"

class Entity
{
public:
	Entity();
	virtual ~Entity(void);
	virtual void update(float dt);
	virtual void render();
	void addChild(Entity* child);
	void removeChild(Entity* child);
	Rect rectWithPos();
	void setCenter(Vec2 pos);
	Vec2 center();

	list<Entity*> children;
	Vec2 pos;
	float rotation;
	Vec2 rotationCenter;
	Vec2 scale;
	Vec2 scaleCenter;
	D3DXMATRIX matrix;
	Rect rect;
	Rect visibleRect;
	bool visible;
	bool updateEnabled;
	bool renderChildrenEnabled;
	Entity* parent;
	bool removing;
	int width;
	int height;
};

