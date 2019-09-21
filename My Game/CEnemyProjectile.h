#pragma once
#include "Object.h"
class CEnemyProjectile :
	public CObject
{
public:
	CEnemyProjectile(const Vector2& p, int dam, float speed, objectTags tag); //Constructor
	CEnemyProjectile(const Vector2 & p, int dam, float speed, objectTags tag, eSpriteType t);
	void Move(); //move the projectile
private:
	float i_SpeedVal; //Speed the projectile moves at
	int i_DamVal; //Damage the projectile does
	Vector2 TargetPos; //Position of player at time of spawn
	Vector2 StartPos; //Position of projectile at time of spawn
	float Distance; //Distance between snot ball and player at spawn
};

