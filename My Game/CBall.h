#pragma once
#include "Object.h"
#include "ObjectManager.h"

class CBall : public CObject {

private:
	float WalkSpeed = 3.0f; /// speed the ball will walk at
	float friction = 0.50f; /// speed the ball will stop at
	float acceleration = 1; /// not being used currently

	int HealthVal = 100;

public:
	CBall(); ///< Default constructor.
	//CBall(const Vector2 & p, eSpriteType sprite);
	CBall(const Vector2& p); ///< Constructor.
	~CBall();
	virtual void Move(); ///< Move object.
	//CObjectManager* m_pObjectManager;
	bool inRange = false; /// true when the player is in range to hit
	bool inMotion = false; /// Bool that represents when the ball is moving
	bool m_bDead = false; ///< Is dead or not.
	BoundingSphere m_Sphere; ///< Bounding sphere.
	Vector2 m_vRadius;
	objectTags Tag; //The type of object for the current object
	BoundingBox m_AabbxVel; ///< Axially aligned bounding box offset by the x velocity of player.
	BoundingBox m_AabbyVel; ///< Axially aligned bounding box offset by the x velocity of player.
	BoundingBox m_signAabbxVel;/// Used for pixel perfect X Collision
	BoundingBox m_signAabbyVel; /// Used for pixel perfect Y Collision 
	Vector2 DirectionNormal; /// vector of ball traveling normailized.
	void XCollisionResponse();
	void YCollisionResponse();
	Vector2 GetPos();
	bool m_bCollision = false;
	Vector2 m_vVel;
	void TakeDamage(int hurtval); //Function that lets the player take incoming damage
	int GetHP(); //Get hp value
	void CollisionCheck();
	float xVel = 0; /// x Velocity for moving player
	float yVel = 0; /// y Velocity for moving player
	float DirAngle = 0;
	float m_fPlayertoBallAngle = 0;
	float range = 150;
	float Charge = 0;
	

};

