/// \file Object.h
/// \brief Interface for the game object class CObject.

#pragma once

#include "GameDefines.h"
#include "Renderer.h"
#include "Common.h"
#include "Component.h"
#include "SpriteDesc.h"

/// \brief The game object. 
///
/// CObject is the abstract representation of an object.

class CObject:
  public CCommon,
  public CComponent,
  public CSpriteDesc2D
{
  friend class CObjectManager;

  protected:
    bool m_bDead = false; ///< Is dead or not.
	BoundingSphere m_Sphere; ///< Bounding sphere.

	

  public:
	CObject(eSpriteType t, const Vector2& p, objectTags tag = OBJECT); ///< Constructor.
	Vector2 m_vRadius;

	virtual void Move(); ///< Move object.
    void kill(); ///< Kill me.
    bool IsDead(); ///< Query whether dead.
	float currentTime; /// used for holding the current time of objects for calcualtions of delta time
	bool isIdle; /// hold whether idle.
	float depth = 0; /// this is used for drawing the objects in order of depth rather than z axis.
	virtual void CollisionResponse(CObject* other); ///< Collision response.
	const BoundingBox& GetBoundingBox(); ///< Get AABB.
	BoundingBox m_Aabb; ///< Axially aligned bounding box.
	objectTags Tag; //The type of object for the current object
}; //CObject