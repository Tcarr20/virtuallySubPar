#include "CLeone.h"
#include "CEnemy.h"
#include "CPlayer.h"
#include "ComponentIncludes.h"

/*
--------------------------------------------+
Default Constructor							|
Takes values for position, amount of health.|
amount of damage, movement speed, and tag	|
--------------------------------------------+
*/
CLeone::CLeone(const Vector2& p, int health, int dam, int speed, objectTags tag) :
	CEnemy(p, health, dam, speed, tag)
{
	e_HealthVal = health;
	e_DamageVal = dam;
	i_SpeedVal = (float)speed;
	Tag = tag;
	canSee = false;
	sALIVE = true;
	sWAITING = true;
	sRUNNING = false;
	sATTACKING = false;
	m_nSpriteIndex = LeoneIdle;

	m_pRenderer->GetSize(Testsprite, m_vRadius.x, m_vRadius.y);
	m_vRadius *= 0.5f;
	const Vector2 topleft(-m_vRadius.x, m_vRadius.y);
	const Vector2 bottomrt(m_vRadius.x, -m_vRadius.y);
	BoundingBox::CreateFromPoints(m_Aabb, topleft + m_vPos, bottomrt + m_vPos);


}

/*
------------------------+
Destructor				|
------------------------+
*/

/*
----------------------------------------------------------------------------+
Function:    Move															|
Parameters:  A pointer to the player										|
Return:      None															|
Description: This function determines the new position for a fi leone enemy	|
			 if the player is not near dont move, but if the player is near |
			 rush towards them.												|
----------------------------------------------------------------------------+
*/
void CLeone::Move()
{
	//Store player and enemy positions into vectors
	Vector2 PlayerPos = Vector2(Target->m_vPos.x, Target->m_vPos.y);
	Vector2 EnemyPos = Vector2(m_vPos.x, m_vPos.y);

	//Get distance between the player and enemy
	float Distance = PlayerPos.Distance(PlayerPos, EnemyPos);

	if((sRUNNING) && (sALIVE)) //Run toward the player like a maniac
	{
		xVel = ((Target->m_vPos.x - m_vPos.x) / Distance) * i_SpeedVal;
		yVel = ((Target->m_vPos.y - m_vPos.y) / Distance) * i_SpeedVal;
		//Apply speed

		//Update bounding box
		const Vector2 topleft(-m_vRadius.x, m_vRadius.y);
		const Vector2 bottomrt(m_vRadius.x, -m_vRadius.y);
		BoundingBox::CreateFromPoints(m_Aabb, topleft + m_vPos, bottomrt + m_vPos);


		const Vector2 xVelocity(xVel, 0);
		const Vector2 yVelocity(0, yVel);
		BoundingBox::CreateFromPoints(m_AabbxVel, topleft + m_vPos + xVelocity, bottomrt + m_vPos + xVelocity);
		BoundingBox::CreateFromPoints(m_AabbyVel, topleft + m_vPos + yVelocity, bottomrt + m_vPos + yVelocity);

		if (m_pObjectManager->m_pTileManager->CollideWithWall((m_AabbxVel))) {
			XCollisionResponse();
		}
		if (m_pObjectManager->m_pTileManager->CollideWithWall((m_AabbyVel))) {
			YCollisionResponse();
		}


		m_vPos.x += xVel;
		m_vPos.y += yVel;
		depth = m_vPos.y; //Fix renderlist position
	}

	//Attack player if close enough
	if ((sATTACKING) && (sALIVE)) 
	{
		if (attackTimer == 0) //If the enemy hasnt already attacked recently
		{
			m_pAudio->play(LEONESPIN, 50.0f);
			attackTimer = 150; //Set attack time to default value
			((CPlayer*)Target)->TakeDamage(e_DamageVal); //Damage the player
		}
	}

	if (!sALIVE)
	{
		//Play death animation
		m_pAudio->play(LEONEDIE, 50.0f);
	}
}

/*
----------------------------------------------------------------------------+
Function:    ProcessState													|
Parameters:  A pointer to the player										|
Return:      None															|
Description: This function updates Fi Leone's states						|
----------------------------------------------------------------------------+
*/
void CLeone::ProcessState()
{
	//Decrement attack timer if not zero
	if (attackTimer != 0)
	{
		attackTimer -= 1;
	}
	//Store player and enemy positions into vectors
	Vector2 PlayerPos = Vector2(Target->m_vPos.x, Target->m_vPos.y);
	Vector2 EnemyPos = Vector2(m_vPos.x, m_vPos.y);
	Vector2 BallPos = m_pBall->m_vPos;

	//Get distance between the player and enemy
	float Distance = PlayerPos.Distance(PlayerPos, EnemyPos);
	float BallDistance = BallPos.Distance(BallPos, EnemyPos);

	if (BallDistance < 25)
	{
		Stroke--;
		TotalStrokes--;
		TakeDamage(1);
	}

	if (Distance < 100)
	{
		if (m_pPlayer->keyHit)
		{
			TakeDamage(1);
		}
	}

	//Check player proximity, if player close, set Fi Leone to run towards the player		
	if ((Distance < 450) && (sALIVE))
	{	
		sRUNNING = true;
		sWAITING = false;
	}
	else if(sALIVE) //If player is not close enough, have Fi Leone wait
	{	
		sRUNNING = false;
		sWAITING = true;
	}

	//If Fi Leone is running towards the player, and if they are close enough, attack
	if((Distance < 45) && (sALIVE))
	{
		sATTACKING = true;
		sRUNNING = false;
	}
	else if(sALIVE) //If the player is not close enough, stop attacking
	{
		sATTACKING = false;
	}

	//Check if Fi Leone has enough health to stay alive
	if (e_HealthVal == 0)
	{
		sALIVE = false;
		sWAITING = false;
		sRUNNING = false;
		sATTACKING = false;
		m_bDead = true;
		for (int i = 0; i < 10; i++)
		{
			CParticleDesc2D par;
			par.m_nSpriteIndex = DUST_SPRITE;
			par.m_vVel.y = -7.0f;
			par.m_fLifeSpan = 1.0f;
			par.m_fScaleOutFrac = 1;
			par.m_fFadeOutFrac = 1;
			par.m_fAlpha = .5f;
			par.m_fRoll = (float)d2r((float)(rand() % 360));
			Vector2 dustspwanpos = Vector2(m_vPos.x + (rand() % 65 + (-32)), m_vPos.y + (rand() % 65 + (-32)));// generate a vector randomly around the ball
			par.m_vPos = dustspwanpos;
			m_pParticleEngine->create(par);
		}
	}
	//Update to appropriate sprite
	if (sWAITING)
	{
		m_nCurrentFrame = 0;
		m_nSpriteIndex = LeoneIdle;
	}
	else if (sRUNNING)
	{
		m_pAudio->play(LEONEMEOW);
		m_nCurrentFrame = 0;
		m_nSpriteIndex = LeoneRun;
	}
	else if (sATTACKING)
	{
		m_nSpriteIndex = LeoneAttack;
		if (m_pTimer->elapsed(currentTime, m_pTimer->frametime() * 2))
		{
			m_nCurrentFrame = (m_nCurrentFrame + 1) % 3;
		}
	}
}

void CLeone::XCollisionResponse()
{
	//collision handleing
	float vxHold = xVel;
	xVel = 0;
	const Vector2 topleftC(-m_vRadius.x + float(fsign(vxHold) * 2), m_vRadius.y);
	const Vector2 bottomrtC(m_vRadius.x + float(fsign(vxHold) * 2), -m_vRadius.y);
	BoundingBox::CreateFromPoints(m_signAabbxVel, topleftC + m_vPos, bottomrtC + m_vPos);
	for (int i = 0; i < abs(vxHold); i++)
	{
		if (!(m_pObjectManager->m_pTileManager->CollideWithWall((m_signAabbxVel))))
		{
			BoundingBox::CreateFromPoints(m_signAabbxVel, topleftC + m_vPos, bottomrtC + m_vPos);
			m_vPos.x += fsign(vxHold);

		}
	}
	//xVel = -vxHold;
}

void CLeone::YCollisionResponse()
{
	float vyHold = yVel;
	yVel = 0;
	const Vector2 topleftC(-m_vRadius.x, m_vRadius.y + float(fsign(vyHold) * 2));
	const Vector2 bottomrtC(m_vRadius.x, -m_vRadius.y + float(fsign(vyHold) * 2));
	BoundingBox::CreateFromPoints(m_signAabbyVel, topleftC + m_vPos, bottomrtC + m_vPos);
	for (int i = 0; i < abs(vyHold); i++)
	{
		if (!(m_pObjectManager->m_pTileManager->CollideWithWall((m_signAabbyVel))))
		{
			BoundingBox::CreateFromPoints(m_signAabbyVel, topleftC + m_vPos, bottomrtC + m_vPos);
			m_vPos.y += fsign(vyHold);

		}
	}
	//yVel = -vyHold;
}


