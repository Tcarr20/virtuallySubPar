#pragma once
#ifndef CBUGS_H
#define CBUGS_H
#include "CEnemy.h"

class CBugs : public CEnemy
{
	friend class CObjectManager;

	protected:
		/*State Variables*/
		bool sWAITING = true; //True when the player is NOT close enought the be hit / False if they are
		bool sATTACKING = false; //True when the player is close enough to be hit / False when the player is too far away 
		bool sALIVE = true; //True when the enemy has more than 0 health / False when the enemy has 0 health
		int attackTimer = 0; //Timer that prevents attack spam
	public:
		CBugs(const Vector2& p, int health, int dam, int speed, objectTags tag); //Constructor
		void Move(); //Controls Bogey Bug's behaviors
		void ProcessState(); //Update Bug's states once per frame
		~CBugs();
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
