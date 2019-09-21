#include "CEnemy.h"

/*
----------------------------+
Enemy Constructor			|
----------------------------+
*/
CEnemy::CEnemy(const Vector2& p, int health, int dam, int speed, objectTags tag) :
	CObject(Testsprite, p, LEONE) {
	e_DamageVal = dam;
	e_HealthVal = health;
	i_SpeedVal = (float)speed;
	Tag = tag;
	canSee = false;


	
} //constructor

/*
----------------------------+
Deconstructor				|
----------------------------+
*/
CEnemy::~CEnemy() {

} //destructor

/*
----------------------------+
Health Getter function		|
----------------------------+
*/
int CEnemy::getHealth()
{
	return e_HealthVal;
}

/*
----------------------------+
Damage Getter function		|
----------------------------+
*/
int CEnemy::getDamage()
{
	return e_DamageVal;
}

/*
----------------------------+
Target Setter function		|
----------------------------+
*/
void CEnemy::setTarget(CObject* Targ)
{
	Target = Targ;
}

/*
------------------------------------------------------------------------+
Function:    TakeDamage													|
Parameters:  An integer the represents amount of damage to be taken		|
Return:      None														|
Description: Function that takes damage and lowers enemy health			|
------------------------------------------------------------------------+
*/
void CEnemy::TakeDamage(int Hurt)
{		
	e_HealthVal -= Hurt;

	if (e_HealthVal < 0)
	{
		e_HealthVal = 0;

	}

}

