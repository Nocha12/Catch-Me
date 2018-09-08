#pragma once
#include "Object.h"
#include "Timer.h"

class Unit :
	public Object
{
public:
	Unit();
	~Unit();
	
	void update(float dt);

	int moveSpeed;

	Vec2 finalMovePos;
	map<AnimationType, Entity*> animationMap;

	void ChangeAnimation(AnimationType type, bool resetAni = false);

	void SetWalk(Vec2 movePos);
	void SetCurrentTile();
	void Move(float dt);
	void Attacked(float backAngle, float backSpeed);

	void astar(Tile *target);
	list<Tile*> routeList;
	Vec2 movePos;
	bool canMove;
	bool stand;
	int attackFrame;
	float attackRange;

	bool attacked;
	bool isAttacked;
	float backAngle;
	float backSpeed;
	Timer deadTimer;
};

