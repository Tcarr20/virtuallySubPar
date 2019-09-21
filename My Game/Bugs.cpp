#include "Bugs.h"
#include "ComponentIncludes.h"
#include "CPlayer.h"

/*Constructor*/
CBugs::CBugs(const Vector2& p, int health, int dam, int speed, objectTags tag) :
	CEnemy(p, health, dam, speed, tag)
{
	e_HealthVal = health;
	e_DamageVal = dam;
	i_SpeedVal = (float)speed;
	Tag = tag;
	m_nSpriteIndex = BugIdle;
	depth = -1000;

	m_pRenderer->GetSize(Testsprite, m_vRadius.x, m_vRadius.y);
	m_vRadius *= 0.5f;
	const Vector2 topleft(-m_vRadius.x, m_vRadius.y);
	const Vector2 bottomrt(m_vRadius.x, -m_vRadius.y);
	BoundingBox::CreateFromPoints(m_Aabb, topleft + m_vPos, bottomrt + m_vPos);
}

/*
	Move the enemy based on state
	: Params : Pointer to player object
*/
void CBugs::Move()
{
	//If attacking, throw snot balls at player
	if (sATTACKING)
	{
		m_pAudio->play(BUGBUZZ, 0.2f);

		if (attackTimer == 0) 
		{
			m_pAudio->play(SHOOTSNOT, 5.0f);
			attackTimer = 175; //Set attack timer
			m_pObjectManager->create(BugProject, Vector2(m_vPos.x, m_vPos.y), EPROJECT); //Shoot a projectile at the player
		}
	}

	//Death condition
	if (!sALIVE)
	{
		//Play death animation
		m_pAudio->play(BUGDIE, 10.0f);
	}
}

/*
	Determine the state of the enemy
	: Params : Pointer to the player object
*/
void CBugs::ProcessState()
{
	//Decrement attackTimer if not zero
	if (attackTimer != 0)
	{
		attackTimer -= 1;
	}

	//Store player and enemy positions into vectors
	Vector2 PlayerPos = Target->m_vPos;
	Vector2 EnemyPos = m_vPos;
	Vector2 BallPos = m_pBall->m_vPos;

	//Get distance between the player and enemy
	float Distance = PlayerPos.Distance(PlayerPos, EnemyPos);
	float BallDistance = BallPos.Distance(BallPos, EnemyPos);

	if (BallDistance < 30)
	{
			Stroke--;
			TotalStrokes--;
			TakeDamage(1);
	}

	//If player is close enough for Bogey Bug to attack
	if ((Distance < 350) && (sALIVE))
	{
		sATTACKING = true;
		sWAITING = false;

	}
	else if (sALIVE) //If not close enough, cannot attack
	{
		sATTACKING = false;
		sWAITING = true;
	}

	//Check if Bogey Bug has enough health to stay alive
	if (e_HealthVal == 0)
	{
		sALIVE = false;
		sWAITING = false;
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

	if (sWAITING) //While idle, play idle animation
	{
		m_nSpriteIndex = BugIdle;
		if (m_pTimer->elapsed(currentTime, m_pTimer->frametime() * 4))
		{
			m_nCurrentFrame = (m_nCurrentFrame + 1) % 3;
		}
	}

	if (sATTACKING) //While attacking, play attack animation
	{
		m_nSpriteIndex = BugAttack;
		if (m_pTimer->elapsed(currentTime, m_pTimer->frametime() * 2))
		{
			m_nCurrentFrame = (m_nCurrentFrame + 1) % 3;
		}
	}
}

