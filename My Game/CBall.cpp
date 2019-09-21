#include "CBall.h"
#include "ComponentIncludes.h"

CBall::CBall() :
CBall(Vector2::Zero) {	
}//CBall

CBall::CBall(const Vector2& p):// eSpriteType sprite) :
	CObject(GolfBall, p, BALL)
{
	Tag = BALL;
	m_vPos = p;
	m_pObjectManager = CCommon::m_pObjectManager;
	m_pPlayer = CCommon::m_pPlayer;
	m_pRenderer->GetSize(GolfBall, m_vRadius.x, m_vRadius.y);
	m_vRadius *= 0.33f;
	const Vector2 topleft(-m_vRadius.x , m_vRadius.y);
	const Vector2 bottomrt(m_vRadius.x, -m_vRadius.y);
	BoundingBox::CreateFromPoints(m_Aabb, topleft + m_vPos, bottomrt + m_vPos);
	

} //constructor

CBall::~CBall()
{
} //destructor

void CBall::Move()
{
	DirectionNormal = Vector2( xVel, yVel) ;
	DirectionNormal.Normalize();
	DirAngle = atan2( DirectionNormal.y, DirectionNormal.x);
	//m_fRoll = DirAngle;
	depth = m_vPos.y; //Fix renderlist position
	xVel = approach(xVel, 0.0f, friction*abs(DirectionNormal.x)); // the x velocity will always approach 0 by the value of friction
	yVel = approach(yVel, 0.0f, friction*abs(DirectionNormal.y)); // the x velocity will always approach 0 by the value of friction
	float deltaTime = m_pTimer->frametime(); // get the time between frames
	if (!inMotion)
	{

	}

	Vector2 PlayerToBall = Vector2(m_pPlayer->m_vPos - m_vPos);
	float xEr = ((m_pPlayer->m_vPos.x) - m_vPos.x);
	float yEr = ((m_pPlayer->m_vPos.y) - m_vPos.y);
	float Distance = sqrt( (xEr*xEr)+(yEr*yEr) );
	if (Distance < range)
	{
		inRange = true;
	}
	else
	{
		inRange = false;
	}
	m_fPlayertoBallAngle = atan2(PlayerToBall.y, PlayerToBall.x); // get the angle between positions
	if (inRange)
	{

		if (m_pPlayer->keyCharge)
		{
			//if (m_pTimer->elapsed(currentTime, deltaTime * 10))
			//{
				Charge += 0.01f;
				Charge = fclamp(Charge, 3.0f, 0.50f);
			//}
		}

		if (m_pPlayer->keyHit)
		{
			//Get the angle vector between player and ball
			shake = Charge*0.3f;
			xVel = -cos(m_fPlayertoBallAngle)*m_pPlayer->hitStrength*Charge; // set the ball to move correctly 
			yVel = -sin(m_fPlayertoBallAngle)*m_pPlayer->hitStrength*Charge; //
			Charge = 0.50f;
			TotalStrokes++;
			Stroke++;

			if (Stroke == (CoursePar + 1))
			{
				kill();
			}
			
		}
	}
	else {
		//Charge = 1.0f;
	}

	//Update the Velocity bounding boxes
		//Update bounding box
	const Vector2 topleft(-m_vRadius.x, m_vRadius.y);
	const Vector2 bottomrt(m_vRadius.x, -m_vRadius.y);
	BoundingBox::CreateFromPoints(m_Aabb, topleft + m_vPos, bottomrt + m_vPos);


	const Vector2 xVelocity(xVel, 0);
	const Vector2 yVelocity(0, yVel);
	BoundingBox::CreateFromPoints(m_AabbxVel, topleft + m_vPos + xVelocity, bottomrt + m_vPos + xVelocity);
	BoundingBox::CreateFromPoints(m_AabbyVel, topleft + m_vPos + yVelocity, bottomrt + m_vPos + yVelocity);

	CParticleDesc2D par;
	par.m_nSpriteIndex = DUST_SPRITE;
	par.m_vVel.y = -7.0f;
	par.m_fLifeSpan = 0.40f;
	par.m_fScaleOutFrac = 1;
	par.m_fFadeOutFrac = 1;
	par.m_fAlpha = .5f;
	par.m_fRoll = (float)d2r((float)(rand() % 360));
	Vector2 dustspwanpos = Vector2(m_vPos.x+ (rand() % 65 + (-32)), m_vPos.y+ (rand() % 65 + (-32)) );// generate a vector randomly around the ball
	par.m_vPos = dustspwanpos;
	

	if ( m_pObjectManager->m_pTileManager->CollideWithWall((m_AabbxVel))) {
		XCollisionResponse();
		m_pAudio->play(BALLBOUNCE);
		m_pParticleEngine->create(par);

	}
	if ( m_pObjectManager->m_pTileManager->CollideWithWall((m_AabbyVel))) {
		YCollisionResponse();
		m_pAudio->play(BALLBOUNCE);
		m_pParticleEngine->create(par);
	}
	

	m_vPos.x += xVel; // finally move the x pos of the object (Ball)
	m_vPos.y += yVel; // finally move the y pos of the object (Ball)


	
}

void CBall::XCollisionResponse()
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
	xVel = -vxHold;
}

void CBall::YCollisionResponse()
{
	float vyHold = yVel;
	yVel = 0;
	const Vector2 topleftC(-m_vRadius.x , m_vRadius.y+ float(fsign(vyHold) * 2));
	const Vector2 bottomrtC(m_vRadius.x , -m_vRadius.y + float(fsign(vyHold) * 2));
	BoundingBox::CreateFromPoints(m_signAabbyVel, topleftC + m_vPos, bottomrtC + m_vPos);
	for (int i = 0; i < abs(vyHold); i++)
	{
		if (!(m_pObjectManager->m_pTileManager->CollideWithWall((m_signAabbyVel))))
		{
			BoundingBox::CreateFromPoints(m_signAabbyVel, topleftC + m_vPos, bottomrtC + m_vPos);
			m_vPos.y += fsign(vyHold);

		}
	}
	yVel = -vyHold;
}

Vector2 CBall::GetPos()
{
	return Vector2();
}

void CBall::TakeDamage(int hurtval)
{
	HealthVal -= hurtval;

	if (HealthVal <= 0)
	{
		HealthVal = 0;
		kill();
	}

}

int CBall::GetHP()
{
	return HealthVal;
}

void CBall::CollisionCheck()
{
	if (m_pObjectManager->m_pTileManager->CollideWithWall((m_AabbxVel))) {
		XCollisionResponse();
	}
	if (m_pObjectManager->m_pTileManager->CollideWithWall((m_AabbyVel))) {
		YCollisionResponse();
	}
	return;

}

