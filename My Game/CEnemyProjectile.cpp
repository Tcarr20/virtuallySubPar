#include "CEnemyProjectile.h"
#include "CPlayer.h"


/*
Default Constructor
*/
CEnemyProjectile::CEnemyProjectile(const Vector2& p, int dam, float speed, objectTags tag) :
	CObject(BugProject, p, EPROJECT)
{
	i_SpeedVal = speed;
	i_DamVal = dam;
	Tag = tag;

	TargetPos = Vector2(m_pPlayer->m_vPos.x, m_pPlayer->m_vPos.y);
	StartPos = Vector2(m_vPos.x, m_vPos.y);

	Distance = TargetPos.Distance(TargetPos, StartPos);
}

CEnemyProjectile::CEnemyProjectile(const Vector2& p, int dam, float speed, objectTags tag,eSpriteType t) :
	CObject(BugProject, p, EPROJECT)
{
	i_SpeedVal = speed;
	i_DamVal = dam;
	Tag = tag;
	m_nSpriteIndex = t;
	TargetPos = Vector2(m_pPlayer->m_vPos.x, m_pPlayer->m_vPos.y);
	StartPos = Vector2(m_vPos.x, m_vPos.y);

	Distance = TargetPos.Distance(TargetPos, StartPos);
}

/*
Move
: Description : Move the projectile towards the player each frame
*/
void CEnemyProjectile::Move()
{
	//Apply speed
	m_vPos.x += (TargetPos.x - StartPos.x)/Distance * i_SpeedVal;
	m_vPos.y += (TargetPos.y - StartPos.y)/Distance * i_SpeedVal;
	depth = m_vPos.y; //Fix renderlist position
	m_fRoll+=0.1f;

	if (m_vPos == TargetPos)
	{
		kill();
	}

	//Update bounding box
	const Vector2 topleft(-m_vRadius.x, m_vRadius.y);
	const Vector2 bottomrt(m_vRadius.x, -m_vRadius.y);
	BoundingBox::CreateFromPoints(m_Aabb, topleft + m_vPos, bottomrt + m_vPos);

	if (m_pObjectManager->m_pTileManager->CollideWithWall((m_Aabb))) {
		kill();
	}

	
}

