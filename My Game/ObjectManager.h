/// \file ObjectManager.h
/// \brief Interface for the object manager CObjectManager.

#pragma once

#include <list>

#include "Object.h"

#include "Component.h"
#include "Common.h"
#include "Settings.h"
#include "CPlayer.h"
#include "CEnemy.h"
#include "CLeone.h"
#include "Bugs.h"
#include "CDozer.h"
#include "TileManager.h"
#include "CBall.h"
#include "CEnemyProjectile.h"
#include "CGoal.h"
#include "CHand.h"
#include "CHead.h"

using namespace std;

/// \brief The object manager.
///
/// A collection of all of the game objects.

class CObjectManager: 
  public CComponent, 
  public CCommon, 
  public CSettings{

  private:
	  

  public:
	vector<CObject*> m_stdObjDrawVec; ///< Object vector for drawing becuase the list is sorting out of our control
    CObjectManager(); ///< Constructor.
    ~CObjectManager(); ///< Destructor.
	void BroadPhase(); ///< Broad phase collision detection and response.

    CObject* create(eSpriteType t, const Vector2& v, objectTags Ta = OBJECT); ///< Create new object.

    void clear(); ///< Reset to initial conditions.
    void move(CObject* Player); ///< Move all objects.
    void draw(); ///< Draw all objects
	list<CObject*> m_stdObjectList; ///< Object list.


	void NarrowPhase(CObject* p0, CObject* p1); ///< Narrow phase collision detection and response.
	void CullDeadObjects(); ///< Cull dead objects..
	void LoadMap(int level); ///< Load a level.
	void GenMaps(int level); /// generate the levels
	//void createEntites(); /// loads the player enimes and ball in place.
	CTileManager* m_pTileManager = nullptr; ///< Pointer to tile manager.
	
	
}; //CObjectManager