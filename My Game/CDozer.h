#pragma once
#ifndef CDOZER_H
#define CDOZER_H
#include "CEnemy.h"

class CDozer : public CEnemy
{
friend class CObjectManager;

protected:
	/*State Variables*/
	bool sWAITING = true; //True when the player is NOT close enought the be hit / False if they are
	bool sMOVING = false; //True when Baby Dozer is close enough to see the ball / False if they cannot see it
	bool sPUSHING = false; //True when Baby Dozer has the ball and is pushing it towards danger / False if it does NOT have the ball 
	bool sALIVE = true; //True when the enemy has more than 0 health / False when the enemy has 0 health
	float MaxUp;
	float MinDown;
	Vector2 PreviousPos; //Last position where a turn occured.
	float turnLength = 100;   //The distance travelled before dozer turns
	float attackTimer = 0;
	float turnTimer = 0;
public:
	CDozer(const Vector2& p, int health, int dam, int speed, objectTags tag); //Constructor
	void Move(); //Controls Baby Dozer's behaviors
	void ProcessState(); //Update Baby Dozer's states once per frame
	void Turn(); //change direction

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