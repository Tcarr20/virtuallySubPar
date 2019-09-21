#pragma once
#ifndef Leone_H
#define Leone_H
#include "CEnemy.h"

/*
------------------------------------------------------------------------+
The Class for the Fi Leone enemey										|
																		|
This enemy will wait for the player to get close, and will				|
then dash towards the player, once close enough they will try to slash	|
at the player, then pause for a quick second.							|
------------------------------------------------------------------------+
*/

class CLeone :	public CEnemy
{
friend class CObjectManager;

protected:
	/* State variables*/
	bool sALIVE; //True when the enemy has more than 0 health | False if health is at 0
	bool sWAITING; //True when the player is not near Fi Leone | False if the player is close to be seen
	bool sRUNNING; //True when Fi Leone is running toward the player | False if player is not close enough to be seen
	bool sATTACKING; //True when Fi Leone is close enough to hit the player | False if not close enough to hit
	int attackTimer = 0; //Timer that prevents the enemy from damaging the player too quickly
	
public:
	CLeone(const Vector2& p, int health, int dam, int speed, objectTags tag); //Constructor
	~CLeone(); //Destructor
	void Move(); //Controls Fi Leone's behaviors
	void ProcessState(); //Update Leone's states once per frame

	float xVel;
	float yVel;
	BoundingBox m_AabbxVel;
	BoundingBox m_AabbyVel;
	BoundingBox m_signAabbxVel;/// Used for pixel perfect X Collision
	BoundingBox m_signAabbyVel; /// Used for pixel perfect Y Collision 
	Vector2	m_vRadius;
	void XCollisionResponse();
	void YCollisionResponse();

};
#endif
