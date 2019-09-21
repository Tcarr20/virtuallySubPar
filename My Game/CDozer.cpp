#include "CDozer.h"
#include "CPlayer.h"
#include "ComponentIncludes.h"

/*Constructor to be called by object manager*/
CDozer::CDozer(const Vector2& p, int health, int dam, int speed, objectTags tag) :
	CEnemy(p, health, dam, speed, tag)
{
	e_HealthVal = health;
	e_DamageVal = dam;
	i_SpeedVal = (float)speed;
	Tag = tag;
	m_nSpriteIndex = DozerDown;
	MaxUp = p.y + 50;
	MinDown = p.y - 50;

	m_pRenderer->GetSize(Testsprite, m_vRadius.x, m_vRadius.y);
	m_vRadius *= 0.5f;
	const Vector2 topleft(-m_vRadius.x, m_vRadius.y);
	const Vector2 bottomrt(m_vRadius.x, -m_vRadius.y);
	BoundingBox::CreateFromPoints(m_Aabb, topleft + m_vPos, bottomrt + m_vPos);
}
/*
Move
: Params : A pointer to the player object
: Description : Move the enemy based on state
*/
void CDozer::Move()
{
	//Store ball and enemy positions into vectors
	Vector2 BallPos = Vector2(Target->m_vPos.x, Target->m_vPos.y);
	Vector2 EnemyPos = Vector2(m_vPos.x, m_vPos.y);

	//Get distance between the ball and enemy
	float Distance = BallPos.Distance(BallPos, EnemyPos);

	//Have Baby Dozer move along it's normal path
	if ((sWAITING) && (sALIVE))
	{
		yVel = i_SpeedVal;
		xVel = 0;

		//Update bounding box
		const Vector2 topleft(-m_vRadius.x, m_vRadius.y);
		const Vector2 bottomrt(m_vRadius.x, -m_vRadius.y);
		BoundingBox::CreateFromPoints(m_Aabb, topleft + m_vPos, bottomrt + m_vPos);

		//collide with walls
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

		//Move baby dozer up and down

		m_vPos.y += yVel;
		depth = m_vPos.y; //Fix renderlist position

		//If baby dozer reaches min or max height, then 
		if ((m_vPos.y >= MaxUp) || (m_vPos.y <= MinDown))
		{
			i_SpeedVal = -i_SpeedVal;
		}

	}

	//Have Baby Dozer move toward the ball
	if ((sMOVING) && (sALIVE))
	{
		if (i_SpeedVal < 0)
		{
			i_SpeedVal = -i_SpeedVal;

		}

		xVel = ((Target->m_vPos.x - m_vPos.x) / Distance) * i_SpeedVal;
		yVel = ((Target->m_vPos.y - m_vPos.y) / Distance) * i_SpeedVal;
		


		//Update bounding box
		const Vector2 topleft(-m_vRadius.x, m_vRadius.y);
		const Vector2 bottomrt(m_vRadius.x, -m_vRadius.y);
		BoundingBox::CreateFromPoints(m_Aabb, topleft + m_vPos, bottomrt + m_vPos);

		//collide with walls
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

		//Apply speed
		m_vPos.x += xVel;
		m_vPos.y += yVel;
		depth = m_vPos.y; //Fix renderlist position
	}

	//Have Baby Dozer move toward the nearest trap
	if ((sPUSHING) && (sALIVE))
	{
		if (i_SpeedVal < 0)
		{
			i_SpeedVal = -i_SpeedVal;
		}

		if (turnTimer == 0)
		{
			m_pRandom->srand((int)m_pTimer->frametime());
			int nextTurn = m_pRandom->randn(10, 200);
			turnTimer = (float)nextTurn;
			Turn();
		}

		//Update bounding box of dozer
		const Vector2 topleft(-m_vRadius.x, m_vRadius.y);
		const Vector2 bottomrt(m_vRadius.x, -m_vRadius.y);
		BoundingBox::CreateFromPoints(m_Aabb, topleft + m_vPos, bottomrt + m_vPos);

		//Update bounding box of ball
		const Vector2 targ_topleft(-Target->m_vRadius.x, Target->m_vRadius.y);
		const Vector2 targ_bottomrt(Target->m_vRadius.x, -Target->m_vRadius.y);
		BoundingBox::CreateFromPoints(Target->m_Aabb, targ_topleft + Target->m_vPos, targ_bottomrt + Target->m_vPos);

		//collide with walls
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

		//Apply speed to Dozer
		m_vPos.y += yVel;
		m_vPos.x += xVel;

		if (m_nSpriteIndex == DozerUp)
		{
			depth = m_vPos.y;
		}
		else
		{
			depth = m_vPos.y + 1;
		}
		//Apply speed to Ball
		//((CBall*)Target)->yVel -= i_SpeedVal; // affect y pos with this
		//if (yVel != 0) {
		
			((CBall*)Target)->m_vPos.y += yVel; // don't affect the y pos like this
			((CBall*)Target)->m_vPos.x += xVel;
			((CBall*)Target)->CollisionCheck();
			//Target->depth = Target->m_vPos.y; //Fix renderlist position
		//}

		if (attackTimer == 0) //If the enemy hasnt already attacked recently
		{
			m_pAudio->play(DOSERCRY, 10.0f);
			attackTimer = 40; //Set attack time to default value
			((CBall*)Target)->TakeDamage(e_DamageVal); //Damage the player
		}

	}

	//Death condition
	if (!sALIVE)
	{
		//Death animation goes here
		m_pAudio->play(DOSERDIE, 5.0f);
	}

}
/*
ProcessState
: Params : A pointer to the player object
: Description : Determine the enemy's states
*/
void CDozer::ProcessState()
{
	//Store ball and enemy positions into vectors
	Vector2 BallPos = Vector2(Target->m_vPos.x, Target->m_vPos.y);
	Vector2 EnemyPos = Vector2(m_vPos.x, m_vPos.y);

	//Get distance between the ball and enemy
	float Distance = BallPos.Distance(BallPos, EnemyPos);

	Vector2 PlayerPos = Vector2(m_pPlayer->m_vPos.x, m_pPlayer->m_vPos.y);
	float PlayerDistance = PlayerPos.Distance(PlayerPos, EnemyPos);

	//Decrement attack timer if not zero
	if (attackTimer != 0)
	{
		attackTimer -= 1;
	}
	if (turnTimer != 0)
	{
		turnTimer -= 1;
	}

	if (PlayerDistance < 100)
	{
		if (m_pPlayer->keyHit)
		{
			m_pAudio->play(DOSERHIT);
			TakeDamage(1);
		}
	}

	//If the ball is close to Baby Dozer, move towards it.
	if ((Distance < 350) && (sALIVE))
	{
		sMOVING = true;
		sWAITING = false;
		sPUSHING = false;
	}
	else if (sALIVE) //Otherwise, do wait behavior
	{
		sMOVING = false;
		sWAITING = true;
		sPUSHING = false;
	}

	//If Baby Dozer has the ball
	if ((Distance < 26) && (sALIVE))
	{
		sPUSHING = true;
		sMOVING = false; 
	}

	//Check if Baby Dozer has enough health to stay alive
	if (e_HealthVal == 0)
	{
		sALIVE = false;
		sWAITING = false;
		sMOVING = false;
		sPUSHING = false;
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

	//Manage sprites for Dozer
	if ((fabs(xVel) > fabs(yVel)) && (!sPUSHING)) //Moving in x axis
	{
		if (xVel > 0) //Right
		{
			m_nSpriteIndex = DozerRight;
		}
		else if (xVel < 0) //left
		{
			m_nSpriteIndex = DozerLeft;
		}
	}
	else if ((fabs(yVel) > fabs(xVel)) && (!sPUSHING)) //Moving in y axis
	{
		if (yVel > 0) // Up
		{
			m_nSpriteIndex = DozerUp;
		}
		else if(yVel < 0) //Down
		{
			m_nSpriteIndex = DozerDown;
		}
	}
}

/*
Turn
: Description : Turn Baby dozer 90 degrees
*/
void CDozer::Turn()
{
	int nextDirection = 0;
	PreviousPos = m_vPos;

	m_pRandom->srand((int)m_pTimer->frametime());
	nextDirection = m_pRandom->randn(0, 3);

	switch (nextDirection)
	{
		case 0: //Go left 
			xVel = -i_SpeedVal;
			yVel = 0;
			m_nSpriteIndex = DozerLeft;
			((CBall*)Target)->m_vPos.y = m_vPos.y;
			((CBall*)Target)->m_vPos.x = m_vPos.x - 25.0f;
			((CBall*)Target)->CollisionCheck();
			break; 
		case 1: //Go up
			xVel = 0;
			yVel = i_SpeedVal;
			m_nSpriteIndex = DozerUp;
			((CBall*)Target)->m_vPos.y = m_vPos.y + 25.0f;
			((CBall*)Target)->m_vPos.x = m_vPos.x;
			((CBall*)Target)->CollisionCheck();
			break;
		case 2: //Go right
			xVel = i_SpeedVal;
			yVel = 0;
			m_nSpriteIndex = DozerRight;
			((CBall*)Target)->m_vPos.y = m_vPos.y;
			((CBall*)Target)->m_vPos.x = m_vPos.x + 25.0f;
			((CBall*)Target)->CollisionCheck();
			break;
		case 3: //Go down
			xVel = 0;
			yVel = -i_SpeedVal;
			m_nSpriteIndex = DozerDown;
			((CBall*)Target)->m_vPos.y = m_vPos.y - 25.0f;
			((CBall*)Target)->m_vPos.x = m_vPos.x;
			((CBall*)Target)->CollisionCheck();
			break;
	}/*
	if ((yVel < 0) && (!turned)) //If moving moving down
	{
		//Go left
		xVel = -i_SpeedVal;
		yVel = 0;
		m_nSpriteIndex = DozerLeft;
		((CBall*)Target)->m_vPos.y = m_vPos.y;
		((CBall*)Target)->m_vPos.x = m_vPos.x - 40;
		((CBall*)Target)->CollisionCheck();
		turned = true;
	}

	if ((xVel < 0) && (!turned)) //If moving left 
	{
		//Go up
		xVel = 0;
		yVel = i_SpeedVal;
		m_nSpriteIndex = DozerUp;
		((CBall*)Target)->m_vPos.y = m_vPos.y + 25;
		((CBall*)Target)->m_vPos.x = m_vPos.x;
		((CBall*)Target)->CollisionCheck();
		turned = true;
	}

	if((yVel > 0) && (!turned)) //If moving up
	{ 
		//Go right
		xVel = i_SpeedVal;
		yVel = 0;
		m_nSpriteIndex = DozerRight;
		((CBall*)Target)->m_vPos.y = m_vPos.y;
		((CBall*)Target)->m_vPos.x = m_vPos.x + 40;
		((CBall*)Target)->CollisionCheck();
		turned = true;
	}

	if ((xVel > 0) && (!turned)) //If moving right
	{
		//Go down
		xVel = 0;
		yVel = -i_SpeedVal;
		m_nSpriteIndex = DozerDown;
		((CBall*)Target)->m_vPos.y = m_vPos.y - 25;
		((CBall*)Target)->m_vPos.x = m_vPos.x;
		((CBall*)Target)->CollisionCheck();
		turned = true;
	}
	*/

	return;
}


void CDozer::XCollisionResponse()
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

void CDozer::YCollisionResponse()
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