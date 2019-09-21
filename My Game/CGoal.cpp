#include "CGoal.h"
#include "Common.h"
#include "CBall.h"


CGoal::CGoal(const Vector2& p, objectTags tag) :
	CObject(HOLE, p, GOAL)
{
	depth = m_vPos.y;

	return;
}


void CGoal::Move()
{
	//Store player and enemy positions into vectors
	Vector2 ObjectPos = m_vPos;
	Vector2 BallPos = m_pBall->m_vPos;

	//Get distance between the player and enemy
	float Distance = ObjectPos.Distance(ObjectPos, BallPos);

	if(Distance < 40)
	{
		m_pAudio->play(SINKBALL);
		m_pBall->m_vPos.y = m_vPos.y - 1.0f;
		m_pBall->m_vPos.x = m_vPos.x;
		sACTIVATED = true;
	}

	return;
}

bool CGoal::isActive()
{
	return sACTIVATED;
}

void CGoal::Deactivate()
{
	sACTIVATED = false;
	return;
}
