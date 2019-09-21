#pragma once
#ifndef Hand_H
#define Hand_H
#include "CEnemy.h"

/*
------------------------------------------------------------------------+
The Class for the Fi Hand enemey										|
																		|
This enemy will wait for the player to get close, and will				|
then dash towards the player, once close enough they will try to slash	|
at the player, then pause for a quick second.							|
------------------------------------------------------------------------+
*/

class CHand : public CEnemy
{
	friend class CObjectManager;

protected:
	/* State variables*/
	bool sALIVE; //True when the enemy has more than 0 health | False if health is at 0
	bool sWAITING; //True when the player is not near Fi Hand | False if the player is close to be seen
	bool sRUNNING; //True when Fi Hand is running toward the player | False if player is not close enough to be seen
	bool sATTACKING; //True when Fi Hand is close enough to hit the player | False if not close enough to hit
	int attackTimer = 0; //Timer that prevents the enemy from damaging the player too quickly

public:
	CHand(const Vector2& p, int health, int dam, int speed, objectTags tag,eSpriteType sprite); //Constructor
	~CHand(); //Destructor
	void Move(); //Controls Fi Hand's behaviors
	void ProcessState(); //Update Hand's states once per frame

	float xVel;
	float yVel;
	BoundingBox m_AabbxVel;
	BoundingBox m_AabbyVel;
	BoundingBox m_signAabbxVel;/// Used for pixel perfect X Collision
	BoundingBox m_signAabbyVel; /// Used for pixel perfect Y Collision 
	Vector2	m_vRadius;
	void XCollisionResponse();
	void YCollisionResponse();
	bool Attack = false;
	bool DoneAttacking = false;
	bool Grounded = true;
	float Distance = 0;
	Vector2 PlayerToHand;
	Vector2 Origin;
	int hitCount = 0;
	bool resetattack = true;
	float hitClose = 0;
	bool IamDead = false;
	bool playonce = false;
};
#endif
