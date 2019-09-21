/// \file Object.cpp
/// \brief Code for the game object class CObject.

#include "Object.h"
#include "ComponentIncludes.h"
#include <GamePad.h>
#include "DebugPrintf.h"

std::unique_ptr<GamePad> gamepad;

/// \param t Type of sprite.
/// \param p Initial position of object.

/*
----------------------------------------------------------------+
Standard Object constructor:									|
Takes parameters for a sprite, a position, and a tag,			|
Tag is OBJECT by default										|
----------------------------------------------------------------+
*/
CObject::CObject(eSpriteType t, const Vector2& p, objectTags tag){
  m_nSpriteIndex = t;
  m_vPos = p;
  currentTime = m_pTimer->time();
  isIdle = true;
  depth = p.y;
  Tag = tag;


  m_pRenderer->GetSize(t, m_vRadius.x, m_vRadius.y);
  m_vRadius *= 0.5f;

  const Vector2 topleft(-m_vRadius.x, m_vRadius.y);
  const Vector2 bottomrt(m_vRadius.x, -m_vRadius.y);
  BoundingBox::CreateFromPoints(m_Aabb, topleft + m_vPos, bottomrt + m_vPos);

} //constructor

/// The only object in this game is the text wheel, which
/// slowly rotates. This is achieved by adding a small amount
/// to its roll angle, proportional to frame time.

void CObject::CollisionResponse(CObject* other) {
	
} //CollisionResponse

void CObject::Move(){ 
	
	const Vector2 topleft(-m_vRadius.x, m_vRadius.y);
	const Vector2 bottomrt(m_vRadius.x, -m_vRadius.y);
	BoundingBox::CreateFromPoints(m_Aabb, topleft + m_vPos, bottomrt + m_vPos);
} //move

/// Kill an object by marking its "is dead" flag. The object
/// will get deleted later at the appropriate time.

void CObject::kill(){
  m_bDead = true;
} //kill

/// Reader function for the "is dead" flag.
/// \return true if marked as being dead, that is, ready for disposal.

bool CObject::IsDead(){
  return m_bDead;
} //IsDead

const BoundingBox& CObject::GetBoundingBox() {
	return m_Aabb;
} //GetBoundingBox

