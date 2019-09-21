#pragma once
#include "Object.h"

class CPlayer : public CObject {
	private:
		int WalkSpeed = 3; /// speed the player will walk at
		float friction = 0.5f; /// speed the player will stop at
		float acceleration = 1; /// not being used currently
		float xVel = 0; /// x Velocity for moving player
		float yVel = 0; /// y Velocity for moving player
	
	public:
		CPlayer(); ///< Default constructor.
		CPlayer(const Vector2& p); ///< Constructor.
		~CPlayer(); ///< Destructor.


		virtual void Move(); ///< Move object.




};