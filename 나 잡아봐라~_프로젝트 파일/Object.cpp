#include "DXUT.h"
#include "Object.h"

Object::Object() : moveRotation(0), attackTarget(nullptr), currentTile(nullptr), colRange(0), isUnit(false), collisionAble(true)
{
}

Object::~Object()
{
}

Vec2 Object::GetCollisionPos(){ return pos + collisionPos; }
void Object::SetCollisionPos(Vec2 pos){ this->pos = pos - collisionPos; }
