/// \file Common.cpp
/// \brief Code for the class CCommon.
///
/// This file contains declarations and initial values
/// for CCommon's static member variables.

#include "Common.h"

CRenderer* CCommon::m_pRenderer = nullptr;
CObjectManager* CCommon::m_pObjectManager = nullptr;
CParticleEngine2D* CCommon::m_pParticleEngine = nullptr;
CBall* CCommon::m_pBall = nullptr;
CPlayer* CCommon::m_pPlayer = nullptr;
CGoal* CCommon::m_pGoal = nullptr; /// pointer to  the ball
bool CCommon::m_bDrawAABBs = false;
Vector2 CCommon::m_vWorldSize = Vector2::Zero;
Vector2 CCommon::m_vScreenShake = Vector2::Zero;
int CCommon::Stroke = 0;
int CCommon::TotalStrokes = 0;
int CCommon::CoursePar = 0;
float CCommon::shake = 0.0f;

eSoundType CCommon::m_eDownSnd = CLANG_SOUND;
eSoundType CCommon::m_eUpSnd = UMPH_SOUND;
eSoundType CCommon::levelMusic = FORTHECOURSE;
eSoundType CCommon::underPar = UNDERPAR;

float approach(float start, float end, float difference)
{
	float result;
	if (start < end) {
		result = min(start + difference, end);
	}
	else {
		result = max(start - difference, end);
	}
	return result;
}

float fsign(float value)
{
	if (value > 0) { return 1; }
	else if (value < 0) { return -1; }
	else { return 0; }
}

float fclamp(float value, float high, float low)
{
	if (value > high) { return high; }
	else if (value < low) { return low; }
	else { return value; }
}
float d2r(float degree)
{
	return (degree *(XM_PI / 180));
}

