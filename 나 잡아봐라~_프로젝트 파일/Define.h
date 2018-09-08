#pragma once
#pragma warning(disable:4244 4800)
#include "DXUT.h"
#include "Enum.h"

#if defined(DEBUG) || defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

using namespace std;
using namespace std::tr2::sys;

#define Vec2(x,y) D3DXVECTOR2((x),(y))
typedef D3DXVECTOR2 Vec2;

#define for_iter(iter, collection) for(auto iter = collection.begin(); iter != collection.end(); iter++)

#define TILEWIDTH 160
#define TILEHEIGHT 116
#define TILECOUNT 60
#define MAPWIDTH TILEWIDTH * TILECOUNT
#define MAPHEIGHT TILEHEIGHT * TILECOUNT

class Rect:public RECT
{
public:
	Rect()
	{
		left = 0;
		top = 0;
		right = 0;
		bottom = 0;
	}

	Rect(int left, int top, int right, int bottom)
	{
		this->left = left;
		this->top = top;
		this->right = right;
		this->bottom = bottom;
	}

	bool intersects(Rect r, Rect* intersection = nullptr)
	{
		return IntersectRect(intersection, this, &r);
	}

	int width()
	{
		return right - left;
	}

	int height()
	{
		return bottom - top;
	}

	Vec2 center()
	{
		return Vec2(width(), height())/2;
	}

	Rect offset(Vec2 pos)
	{
		Rect r = *this;
		r.left += pos.x;
		r.right += pos.x;
		r.top += pos.y;
		r.bottom += pos.y;
		return r;
	}

	void cutOff(Rect r)
	{
		left += r.left;
		right -= r.right;
		top += r.top;
		bottom -= r.bottom;
	}

	bool intersects(Vec2 pos)
	{
		return pos.x > left && pos.x < right && pos.y > top && pos.y < bottom;
	}

};

inline float angle(Vec2 p1, Vec2 p2)
{
	return atan2(p2.y-p1.y, p2.x-p1.x);
}

inline int random(int from, int to)
{
	static default_random_engine random(timeGetTime());
	uniform_int_distribution<int> d(from, to);
	return d(random);
}

inline float random(float from, float to)
{
	static default_random_engine random(timeGetTime());
	uniform_real_distribution<float> d(from, to);
	return d(random);
}

inline float clamp(float value, float max)
{
	return (value > max)?max:value;
}

inline float lerp(float p1, float p2, float d1) {
	return (1 - d1)*p1 + d1*p2;
}

inline bool circleCollision(Vec2 p1, Vec2 p2, float range)
{
	return pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2) <= range * range;
}

inline float pita(Vec2 p1, Vec2 p2)
{
	return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

enum class Direction : int
{
	TOP,
	LEFT,
	RIGHT,
	BOTTOM
};

inline bool collision(Vec2 p1, Vec2 p2, RECT r1, Rect r2 = Rect(0, 0, 1, 1), RECT *result = nullptr)
{
	r1.left += p1.x;
	r1.right += p1.x;
	r1.top += p1.y;
	r1.bottom += p1.y;

	r2.left += p2.x;
	r2.right += p2.x;
	r2.top += p2.y;
	r2.bottom += p2.y;

	if (result == nullptr)
	{
		RECT temp;
		return IntersectRect(&temp, &r1, &r2);
	}
	else
	{
		return IntersectRect(result, &r1, &r2);
	}
}