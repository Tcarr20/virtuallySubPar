#include "CHand.h"
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
CHand::CHand(const Vector2& p, int health, int dam, int speed, objectTags tag, eSpriteType sprite) :
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
	m_nSpriteIndex = sprite;

	m_pRenderer->GetSize(Testsprite, m_vRadius.x, m_vRadius.y);
	m_vRadius *= 0.5f;
	const Vector2 topleft(-m_vRadius.x, m_vRadius.y -90.0f);
	const Vector2 bottomrt(m_vRadius.x, -m_vRadius.y -90.0f);
	BoundingBox::CreateFromPoints(m_Aabb, topleft + m_vPos, bottomrt + m_vPos);
	Origin = m_vPos;
	


}

/*
------------------------+
Destructor				|
------------------------+
*/
CHand::~CHand()
{
}

/*
----------------------------------------------------------------------------+
Function:    Move															|
Parameters:  A pointer to the player										|
Return:      None															|
Description: This function determines the new position for a fi Hand enemy	|
			 if the player is not near dont move, but if the player is near |
			 rush towards them.												|
----------------------------------------------------------------------------+
*/
void CHand::Move()
{

	if (!IamDead) {
			//Store player and enemy positions into vectors
			Vector2 PlayerPos = Vector2(Target->m_vPos.x, Target->m_vPos.y - 90.0f);
			Vector2 EnemyPos = Vector2(m_vPos.x, m_vPos.y);

			//Get distance between the player and enemy
			Distance = PlayerPos.Distance(PlayerPos, EnemyPos);
			float HomeDistance = Origin.Distance(Origin, EnemyPos);

			if (Attack) //Run toward the player like a maniac
			{
				if (sRUNNING) // if player is in range, go after them
				{
					Vector2 offset = Vector2(m_pPlayer->m_vPos.x, m_pPlayer->m_vPos.y + 90.0f);
					PlayerToHand = Vector2(offset - m_vPos);
					float PlayertoHandAngle = atan2(PlayerToHand.y, PlayerToHand.x); // get the angle between positions
					xVel = (cosf(PlayertoHandAngle)) * i_SpeedVal;
					yVel = (sinf(PlayertoHandAngle)) * i_SpeedVal;
					//m_nCurrentFrame = (int)(approach(m_nCurrentFrame, 4, 1));
					if (m_pTimer->elapsed(currentTime, m_pTimer->frametime() * 5))
					{
						m_nCurrentFrame++;
						m_nCurrentFrame = m_nCurrentFrame % 8;
					}

					//Apply speed
				}
				else // go home
				{
					Vector2 PosToOrigin = (Origin - m_vPos);
					float angletoOrigin = atan2(PosToOrigin.y, PosToOrigin.x);
					xVel = (cosf(angletoOrigin)) * i_SpeedVal;
					yVel = (sinf(angletoOrigin)) * i_SpeedVal;
					m_nCurrentFrame = (int)(approach((float)m_nCurrentFrame, 0, 1));
				}

			}
			else // go home
			{
				Vector2 PosToOrigin = (Origin - m_vPos);
				float angletoOrigin = atan2(PosToOrigin.y, PosToOrigin.x);
				xVel = (cosf(angletoOrigin)) * i_SpeedVal;
				yVel = (sinf(angletoOrigin)) * i_SpeedVal;
				m_nCurrentFrame = (int)(approach((float)m_nCurrentFrame, 0, 1));
			}
			m_nCurrentFrame = m_nCurrentFrame % 8;

			if (m_nCurrentFrame == 7)
			{
				if (resetattack)
				{
					m_pAudio->play(CLANG_SOUND, 0.2f);
					hitCount++;
					resetattack = false;
					hitClose = PlayerToHand.Length();
					if (hitClose < 5)
					{
						((CPlayer*)Target)->TakeDamage(e_DamageVal); //Damage the player
					}
					shake = 0.7f;
				}

			}
			else {
				resetattack = true;
			}

			if (hitCount > 9)
			{
				DoneAttacking = true;
				hitCount = 0;
				
			}

			//Update bounding box
			const Vector2 topleft(-m_vRadius.x, m_vRadius.y - 90.0f);
			const Vector2 bottomrt(m_vRadius.x, -m_vRadius.y - 90.0f);
			BoundingBox::CreateFromPoints(m_Aabb, topleft + m_vPos, bottomrt + m_vPos);

			const Vector2 xVelocity(xVel, 0);
			const Vector2 yVelocity(0, yVel);
			BoundingBox::CreateFromPoints(m_AabbxVel, topleft + m_vPos + xVelocity, bottomrt + m_vPos + xVelocity);
			BoundingBox::CreateFromPoints(m_AabbyVel, topleft + m_vPos + yVelocity, bottomrt + m_vPos + yVelocity);

			if (m_pObjectManager->m_pTileManager->CollideWithWall((m_AabbxVel))) {
				//XCollisionResponse();
			}
			if (m_pObjectManager->m_pTileManager->CollideWithWall((m_AabbyVel))) {
				//YCollisionResponse();
			}

			if (m_nCurrentFrame < 2 || m_nCurrentFrame > 6) // if the hand is on the ground
			{
				Grounded = true;
			}
			else {
				Grounded = false;
			}

			m_vPos.x += xVel;
			m_vPos.y += yVel;
			depth = m_vPos.y; //Fix renderlist position




			//if (Grounded = false)
			//{
			//	if (m_pTimer->elapsed(currentTime, m_pTimer->frametime() * 2))
			//	{
			//		m_nCurrentFrame++;
			//		m_nCurrentFrame = m_nCurrentFrame % 8;
			//		if (Distance <= 45)
			//		{
			//			((CPlayer*)Target)->TakeDamage(e_DamageVal); //Damage the player
			//		}
			//	}

			//}
			//float stuff = PlayerToHand.Length();
			//DebugVal = &(stuff);



		////Attack player if close enough
		//if ((sATTACKING) && (sALIVE))
		//{
		//	if (attackTimer == 0) //If the enemy hasnt already attacked recently
		//	{
		//		//m_pAudio->play(HandSPIN, 50.0f);
		//		//attackTimer = 150; //Set attack time to default value
		//		((CPlayer*)Target)->TakeDamage(e_DamageVal); //Damage the player
		//	}
		//}

		//if (!sALIVE)
		//{
		//	//Play death animation
		//	//m_pAudio->play(HandDIE, 50.0f);
		//}
	}//ifnot dead
	else {
		if (m_pTimer->elapsed(currentTime, m_pTimer->frametime() * 300))
		{
			DoneAttacking = true;
			
		}
		if(playonce== false)
		{
			m_pAudio->vary(BOSSCHUNK);
			playonce = true;
			for (int i = 0; i < 30; i++)
			{
				CParticleDesc2D par;
				par.m_nSpriteIndex = DUST_SPRITE;
				par.m_vVel.y = -7.0f;
				par.m_fLifeSpan = 4.0f;
				par.m_fScaleOutFrac = 1;
				par.m_fFadeOutFrac = 1;
				par.m_fAlpha = .5f;
				par.m_fRoll = (float)d2r((float)(rand() % 360));
				Vector2 dustspwanpos = Vector2(m_vPos.x + (rand() % 129 + (-64)), m_vPos.y + (rand() % 129 + (-64)));// generate a vector randomly around the ball
				par.m_vPos = dustspwanpos;
				m_pParticleEngine->create(par);
			}
		}
	}
}

/*
----------------------------------------------------------------------------+
Function:    ProcessState													|
Parameters:  A pointer to the player										|
Return:      None															|
Description: This function updates Fi Hand's states							|
----------------------------------------------------------------------------+
*/
void CHand::ProcessState()
{
	if (!IamDead)
	{

		//Decrement attack timer if not zero
		//if ((attackTimer <= 0))
		//{
		//	if (Distance < 1)
		//	{
		//		attackTimer = 150;

		//	}
		//}
		//else {
		//	attackTimer--;
		//
		//}
		//Store player and enemy positions into vectors
		Vector2 PlayerPos = Vector2(Target->m_vPos.x, Target->m_vPos.y);
		Vector2 EnemyPos = Vector2(m_vPos.x, m_vPos.y);
		Vector2 BallPos = m_pBall->m_vPos;

		//Get distance between the player and enemy
		float Distance = PlayerPos.Distance(PlayerPos, EnemyPos);
		float BallDistance = BallPos.Distance(BallPos, EnemyPos);

		if (Grounded)
		{
			if (BallDistance < 25)
			{
				TakeDamage(1);
				m_pAudio->vary(BOSSHIT);
			}

			if (Distance < 100)
			{
				if (m_pPlayer->keyHit)
				{
					TakeDamage(1);
					m_pAudio->vary(BOSSHIT);
				}
			}
		}

		//Check player proximity, if player close, set Fi Hand to run towards the player		
		if ((Distance < 450))
		{
			sRUNNING = true;
		}
		else if (sALIVE) //If player is not close enough, have Fi Hand wait
		{
			sRUNNING = false;
		}



		////If Fi Hand is running towards the player, and if they are close enough, attack
		//if ((Distance < 45) && (sALIVE))
		//{
		//	sATTACKING = true;
		//	sRUNNING = false;
		//}
		//else if (sALIVE) //If the player is not close enough, stop attacking
		//{
		//	sATTACKING = false;
		//}

		////Check if Fi Hand has enough health to stay alive
		if (e_HealthVal <= 0)
		{
			sALIVE = false;
			sWAITING = false;
			sRUNNING = false;
			sATTACKING = false;
			//m_bDead = true;
			IamDead = true;
		}
		////Update to appropriate sprite
		//if (sWAITING)
		//{
		//	//m_nCurrentFrame = 0;
		//	//m_nSpriteIndex = HandIdle;
		//}
		//else if (sRUNNING)
		//{
		//	//m_pAudio->play(HandMEOW);
		//	//m_nCurrentFrame = 0;
		//	//m_nSpriteIndex = HandRun;
		//}
		//else if (sATTACKING)
		//{
		//	//m_nSpriteIndex = HandAttack;
		//	if (m_pTimer->elapsed(currentTime, m_pTimer->frametime() * 2))
		//	{
		//		//m_nCurrentFrame = (m_nCurrentFrame + 1) % 3;
		//	}
		//}
	}

}

void CHand::XCollisionResponse()
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

void CHand::YCollisionResponse()
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


