#pragma once
#ifndef Player_H
#define Player_H
#include "Object.h"
#include "ObjectManager.h"
#include "TileManager.h"
#include "ControllerExt.h"

class CPlayer : public CObject {
	private:
		float WalkSpeed = 3.0f; /// speed the player will walk at
		float RunSpeed = 6.0f;
		float speed = WalkSpeed;
		float friction = 0.50f; /// speed the player will stop at
		float acceleration = 1; /// not being used currently
		float xVel = 0; /// x Velocity for moving player
		float yVel = 0; /// y Velocity for moving player
		int HealthVal = 100;
		bool RTTriggerUp = true;
		bool left = false;
		
	
	public:
		CPlayer(); ///< Default constructor.
		CPlayer(const Vector2& p); ///< Constructor.
		~CPlayer(); ///< Destructor.
		virtual void Move(); ///< Move object.
		BoundingBox m_AabbxVel; ///< Axially aligned bounding box offset by the x velocity of player.
		BoundingBox m_AabbyVel; ///< Axially aligned bounding box offset by the x velocity of player.
		virtual void CollisionResponse(CObject* other);
		void XCollisionResponse();
		void YCollisionResponse();
		Vector2 GetPos();
		bool m_bCollision = false;
		Vector2 m_vVel;
		void TakeDamage(int hurtval); //Function that lets the player take incoming damage
		int GetHP();

		float hitStrength = 20;// hitStrength of player

		bool keyLeft = m_pKeyboard->Down('A'); // gather input from keyboard
		bool keyRight = m_pKeyboard->Down('D');
		bool keyUp = m_pKeyboard->Down('W');
		bool keyDown = m_pKeyboard->Down('S');
		bool keyHit = m_pKeyboard->TriggerUp(VK_SPACE);
		bool keyCharge = m_pKeyboard->TriggerUp(VK_UP);
		bool keySprint = false;
		float deadZoneThresh = 0.3f;
		bool RTTriggerUpRealeased = false;
		bool attacking = false;

};
#endif