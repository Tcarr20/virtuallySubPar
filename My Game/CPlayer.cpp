
#include "CPlayer.h"
#include "ComponentIncludes.h"
#include "Helpers.h"
#include "Sndlist.h"
#include "ObjectManager.h"
#include "Game.h"

#include "DebugPrintf.h"
#define USE_DEBUGPRINTF

CPlayer::CPlayer() :
	CPlayer(Vector2::Zero) {
} //CPlayer

CPlayer::CPlayer(const Vector2& p) :
	CObject(PenguinWalkDown, p, PLAYER)
{
	Tag = PLAYER;
	m_pObjectManager = CCommon::m_pObjectManager;

	m_pRenderer->GetSize(PLAYER_MASK, m_vRadius.x, m_vRadius.y);
	m_vRadius *= 0.5f;

	const Vector2 topleft(-m_vRadius.x, m_vRadius.y);
	const Vector2 bottomrt(m_vRadius.x, -m_vRadius.y);
	BoundingBox::CreateFromPoints(m_AabbxVel, topleft + m_vPos, bottomrt + m_vPos);
	BoundingBox::CreateFromPoints(m_AabbyVel, topleft + m_vPos, bottomrt + m_vPos);

	//m_pTileManager = m_pTile;

} //constructor

CPlayer::~CPlayer() {
	
} //destructor


void CPlayer::Move() {
	CObject::Move();

	if (HealthVal == 0)
	{
		m_bDead = true;
	}

	m_pController->GetState();
	
	float deltaTime = m_pTimer->frametime(); // get the time between frames
	bool ADown = ((ControllerExt*)m_pController)->GetButtonADown();
	if (ADown)
	{
		if (RTTriggerUp = true)
		{
			RTTriggerUp = false;
		}
	}
	if (!RTTriggerUp && !ADown)
	{
		RTTriggerUpRealeased = true;
		RTTriggerUp = true;
	}

	keyLeft = (m_pKeyboard->Down('A') || (m_pController->GetLThumb().x <= -deadZoneThresh)); // gather input from keyboard
	keyRight = (m_pKeyboard->Down('D') || (m_pController->GetLThumb().x >= deadZoneThresh));
	keyUp = (m_pKeyboard->Down('W') || (m_pController->GetLThumb().y >= deadZoneThresh));
	keyDown = (m_pKeyboard->Down('S') || (m_pController->GetLThumb().y <= -deadZoneThresh));
	keyHit = (m_pKeyboard->TriggerUp(VK_SPACE) || RTTriggerUpRealeased);
	keyCharge = (m_pKeyboard->Down(VK_SPACE) || ((ControllerExt*)m_pController)->GetButtonADown()  );
	keySprint = (m_pKeyboard->Down(VK_SHIFT) || m_pController->GetLTrigger());


	int xMoveDir = (keyRight - keyLeft);
	int yMoveDir = (keyUp - keyDown);

	if (m_pController->GetButtonAToggle() || m_pKeyboard->TriggerDown(VK_SPACE)) {
		//m_fRoll += 0.125f*XM_2PI*m_pTimer->frametime();  //rotate at 1/8 RPS
	}

	isIdle = true;
	if (keyHit)
	{
		attacking = true;
		m_nCurrentFrame = 0;
	}
	
	if (!attacking)
	{
		if ((xMoveDir == 1)) { // if keyRight only
			m_nSpriteIndex = PenguinWalkRight;
			isIdle = false;
			left = false;

		}
		else if ((xMoveDir == -1)) {  // if keyLeft only
			m_nSpriteIndex = PenguinWalkLeft;
			isIdle = false;
			left = true;
		}
		if ((yMoveDir == 1)) {  // if keyUp only
			m_nSpriteIndex = PenguinWalkUp;
			isIdle = false;
		}
		else if ((yMoveDir == -1)) {  // if keyDown only
			m_nSpriteIndex = PenguinWalkDown;
			isIdle = false;
		}
	}
	else {
		xMoveDir = 0;
		yMoveDir = 0;
	}
	keySprint ? speed = RunSpeed : speed = WalkSpeed;

	if (xMoveDir != 0 || yMoveDir != 0) // if any Move key is pressed
	{
		xVel = (xMoveDir*speed); // set the x velocity to the x move direction (-1,0,1) * the speed 
		yVel = (yMoveDir*speed); // set the y velocity to the y move direction (-1,0,1) * the speed 
	}


	xVel = approach(xVel, 0.0f, friction); // the x velocity will always approach 0 by the value of friction
	yVel = approach(yVel, 0.0f, friction); // the x velocity will always approach 0 by the value of friction
	float offset = 10.0f;
	const Vector2 topleft(-m_vRadius.x, m_vRadius.y-offset);
	const Vector2 bottomrt(m_vRadius.x, -m_vRadius.y - offset);
	const Vector2 xVelocity(xVel, 0);
	const Vector2 yVelocity(0, yVel);
	BoundingBox::CreateFromPoints(m_AabbxVel, topleft + m_vPos + xVelocity, bottomrt + m_vPos + xVelocity);
	BoundingBox::CreateFromPoints(m_AabbyVel, topleft + m_vPos + yVelocity , bottomrt + m_vPos + yVelocity);

	//m_pObjectManager->BroadPhase();


	if (m_pObjectManager->m_pTileManager->CollideWithWall((m_AabbxVel))) {
		XCollisionResponse();
	}
	if (m_pObjectManager->m_pTileManager->CollideWithWall((m_AabbyVel))) {
		YCollisionResponse();
	}

	m_vPos.x += xVel; // finally move the x pos of the object (Player)
	m_vPos.y += yVel; // finally move the y pos of the object (Player)

	float FrameLength;
	keySprint ? FrameLength = 2.3f : FrameLength = 6.0f;
	attacking ? FrameLength = 0.0f : FrameLength = FrameLength;
	

	if (m_pTimer->elapsed(currentTime, deltaTime * FrameLength)) // if the time of 10 frames has passed
	{
		if (!isIdle && !attacking) // if NOT idleing
		{
			m_nCurrentFrame++; // add to the current frame
			m_nCurrentFrame = m_nCurrentFrame % 6; // keep frames inbetween 1 - 6
		}
		else {
			if(attacking)
			{
				if (keyHit){ m_nCurrentFrame = 0; }
					
				m_nSpriteIndex = HIT;
				m_nCurrentFrame++;
				m_nCurrentFrame = m_nCurrentFrame % 6; // keep frames inbetween 1 - 4

			}
			else {

				if (!attacking)
				{
					left ? m_pPlayer->m_nSpriteIndex = PenguinIdleL : m_pPlayer->m_nSpriteIndex = PenguinIdle;
					//m_pPlayer->m_nSpriteIndex = PenguinIdle;
					m_nCurrentFrame++;
					m_nCurrentFrame = m_nCurrentFrame % 4; // keep frames inbetween 1 - 4
				}
			}
			if (m_nCurrentFrame == 5 && attacking)
			{
				attacking = false;
			}

		}
	}
	depth = m_vPos.y; // keep track of the players depth
	m_vVel = Vector2(xVel, yVel);

	//USE_DEBUGPRINTF("%s","HELLO");
	//DEBUGPRINTF("%f",depth);
	RTTriggerUpRealeased = false;
}

void CPlayer::CollisionResponse(CObject* other)
{

	if (other->m_Aabb.Contains(m_AabbxVel)){ // collision Right
		xVel = 0;
	}
	if (other->m_Aabb.Contains(m_AabbxVel)) { // collision Left
		xVel = 0;
	}
	if (other->m_Aabb.Contains(m_AabbyVel)) { // collision UP
		yVel = 0;
	}
	if (other->m_Aabb.Contains(m_AabbyVel)) { // collision Down
		yVel = 0;
	}
}

void CPlayer::XCollisionResponse()
{
	xVel = 0;
}
void CPlayer::YCollisionResponse()
{
	yVel = 0;
}

Vector2 CPlayer::GetPos()
{
	return Vector2(m_vPos.x,m_vPos.y);
}
/*
	TakeDamage : Params :
				- An integer that represents the amount of damage taken
	Description: Reduces player's health based on incoming damage.
*/
void CPlayer::TakeDamage(int hurtval)
{
	HealthVal -= hurtval;

	if (HealthVal < 0)
	{
		HealthVal = 0;
	}
}

int CPlayer::GetHP()
{
	return HealthVal;
}
