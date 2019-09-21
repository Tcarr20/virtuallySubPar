#pragma once
#include "Object.h"
#include "Common.h"
class CGoal :
	public CObject
{
public:
	CGoal(const Vector2& p, objectTags tag);
	void Move();
	bool isActive();
	void Deactivate();
protected:
	bool sACTIVATED = false;
};

