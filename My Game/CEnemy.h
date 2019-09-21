#pragma once
#ifndef Enemy_H
#define Enemy_H
#include "Object.h"

/*
--------------------------------------------------------------------+
Base enemy class, derived from object class. Stores the				|
shared data between all enemies such as health, damage done, etc	|
--------------------------------------------------------------------+
*/
class CEnemy : public CObject
{
friend class CObjectManager;

protected:
	/*External value variables | i.e. Also used outside of class' scope*/
	int e_HealthVal; //The amount of damage an enemy can take | external use
	int e_DamageVal; //The amount of damage an enemy does to the player | external use
	/*Internal value variables | i.e. Only used within class' scope*/
	float i_SpeedVal; //The speed at which an enemy moves | internal only
	CObject* Target; //The target of the enemy 
	bool canSee; //Bool that determines if an enemy can see its target

public:
	CEnemy(const Vector2& p, int health, int dam, int speed, objectTags tag); //Enemy Constructor
	~CEnemy(); //Deconstructor
	int getHealth(); //Get Health Value
	int getDamage(); //Get Damage Value
	void setTarget(CObject* Targ); //Initialize target reference
	void TakeDamage(int Hurt); //Function that decreases enemy health
	virtual void Move() = 0; //Virtual move function
	virtual void ProcessState() = 0; //Virtual state processing function

//	void Move();
};
#endif