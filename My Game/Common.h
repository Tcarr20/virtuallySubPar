/// \file Common.h
/// \brief Interface for the class CCommon.

#pragma once

#include "SndList.h"
#include "Component.h"
#include "ControllerExt.h"
#include "ParticleEngine.h"

//forward declarations to make the compiler less stroppy

class CObjectManager; 
class CRenderer;
class CBall;
class CPlayer;
class CGoal;


/// \brief The common variables class.
///
/// CCommon is a singleton class that encapsulates 
/// things that are common to different game components,
/// including game state variables.
/// Making it a singleton class means
/// that we can avoid passing its member variables
/// around as parameters, which makes the code
/// minisculely faster, and more importantly, reduces
/// function clutter.

class CCommon{
  protected:  
    static CRenderer* m_pRenderer; ///< Pointer to the renderer.

    static eSoundType m_eDownSnd; ///< Key down sound.
    static eSoundType m_eUpSnd; ///< Key up sound.
	static eSoundType levelMusic; //Level music
	static eSoundType underPar; //Title screen music
	static bool m_bDrawAABBs; ///< Whether to draw AABBs.

	public:
		static CObjectManager* m_pObjectManager; ///< Pointer to the object manager.
		static CParticleEngine2D* m_pParticleEngine; ///< Pointer to particle engine.
		static CBall* m_pBall; /// pointer to  the ball
		static CPlayer* m_pPlayer; /// pointer to  the ball
		static CGoal* m_pGoal; /// pointer to  the ball
		static CBall* m_pBall2; /// pointer to  the ball
		static CPlayer* m_pPlayer2; /// pointer to  the ball
		static Vector2 m_vWorldSize; ///< World height and width.
		static Vector2 m_vScreenShake;
		static float shake;
		static int Stroke;
		static int TotalStrokes;
		static int CoursePar;
		



		
}; //CCommon

float approach(float start, float end, float difference);
float fsign(float value);
float fclamp(float value, float high, float low);
float d2r(float degree);


