
#include "Player.h"
#include "ComponentIncludes.h"
#include "Helpers.h"
#include "Sndlist.h"

#include "DebugPrintf.h"

CPlayer::CPlayer() :
	CPlayer(Vector2::Zero) {
} //CPlayer

CPlayer::CPlayer(const Vector2& p) :
	CObject(PenguinWalkDown, p)
{

} //constructor

CPlayer::~CPlayer() {
	
} //destructor


void CPlayer::Move() {
	CObject::Move();
	m_pController->GetState();

	float deltaTime = m_pTimer->frametime(); // get the time between frames

	bool keyLeft	 =   m_pKeyboard->Down('A'); // gather input from keyboard
	bool keyRight	 =   m_pKeyboard->Down('D');
	bool keyUp		 =   m_pKeyboard->Down('W');
	bool keyDown	 =   m_pKeyboard->Down('S');

	int xMoveDir = (keyRight - keyLeft);
	int yMoveDir = (keyUp - keyDown);

	if (m_pController->GetButtonAToggle() || m_pKeyboard->TriggerDown(VK_SPACE)) {
		m_fRoll += 0.125f*XM_2PI*m_pTimer->frametime();  //rotate at 1/8 RPS
	}

	isIdle = true;
	if ((xMoveDir == 1)) { // if keyRight only
		m_nSpriteIndex = PenguinWalkRight;
		isIdle = false;
	}
	else if ((xMoveDir == -1)) {  // if keyLeft only
		m_nSpriteIndex = PenguinWalkLeft;
		isIdle = false;
	}
	if ((yMoveDir == 1)) {  // if keyUp only
		m_nSpriteIndex = PenguinWalkUp;
		isIdle = false;
	}
	else if ((yMoveDir == -1)) {  // if keyDown only
		m_nSpriteIndex = PenguinWalkDown;
		isIdle = false;
	}

	if (xMoveDir != 0 || yMoveDir != 0) // if any Move key is pressed
	{
		xVel = (xMoveDir*WalkSpeed); // set the x velocity to the x move direction (-1,0,1) * the speed 
		yVel = (yMoveDir*WalkSpeed); // set the y velocity to the y move direction (-1,0,1) * the speed 
	}

	xVel = approach(xVel, 0.0f, friction); // the x velocity will always approach 0 by the value of friction
	yVel = approach(yVel, 0.0f, friction); // the x velocity will always approach 0 by the value of friction

	m_vPos.x += xVel; // finally move the x pos of the object (Player)
	m_vPos.y += yVel; // finally move the y pos of the object (Player)


	if (m_pTimer->elapsed(currentTime, deltaTime * 10)) // if the time of 10 frames has passed
	{
		if (isIdle == false) // if NOT idleing
		{
			m_nCurrentFrame++; // add to the current frame
			m_nCurrentFrame = m_nCurrentFrame % 4; // keep frames inbetween 1 - 4
		}
		else {
			m_nCurrentFrame = 2; // if you are idleing then set the frame to a static 2
		}
	}
	depth = m_vPos.y; // keep track of the players depth
	
}