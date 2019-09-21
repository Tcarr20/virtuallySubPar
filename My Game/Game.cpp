/// \file Game.cpp
/// \brief Code for the game class CGame.

#include "Game.h"

#include "GameDefines.h"
#include "Renderer.h"
#include "ComponentIncludes.h"
#include "Object.h"


#include "DebugPrintf.h"

/// Delete the renderer and the object manager.
//CObject Player(PenguinWalkDown, Vector3(0.0f, 0.0f,0.0f));

CGame::~CGame(){
  delete m_pRenderer;
  delete m_pObjectManager;
  delete m_pParticleEngine;
} //destructor

/// Initialize the renderer and the object manager, load 
/// images and sounds, and begin the game.

void CGame::Initialize(){
  m_pRenderer = new CRenderer; 
  m_pRenderer->Initialize(NUM_SPRITES); 
  m_pRenderer->LoadImages(); //load images from xml file list
  m_pRenderer->SetBgColor(Colors::CornflowerBlue ); // set the back ground color
  m_pObjectManager = new CObjectManager; //set up object manager
  m_pParticleEngine = new CParticleEngine2D((CSpriteRenderer*)m_pRenderer);
  Stroke = 0;
  
  m_pAudio->Load(); //load the sounds for this game

  BeginGame();
} //Initialize

/// Ask the object manager to create the game objects.
/// There's only one in this game, the rotating
/// wheel o' text.

void CGame::CreateObjects(){
	if (isMenu)
	{
		m_pObjectManager->create(TITLE, m_vWinCenter, OBJECT);
	}
	else
	{
		createPlayer();
		createBaddies();
	}
	
} //CreateObjects

/// Call this function to start a new game. This
/// should be re-entrant so that you can restart a
/// new game without having to shut down and restart the
/// program, but it's moot here. All we really need
/// to do is clear any old objects out of the object manager
/// and create some new ones.
void CGame::NextLevel(){
  m_nCurLevel++;
  m_nCurLevel = (m_nCurLevel)%(finalLevel + 1);
  BeginGame();
} //NextLevel

void CGame::BeginGame(){  
  m_pAudio->stop();
  m_pObjectManager->clear(); //clear old objects
  //m_pObjectManager->GenMaps(1);//generate a level 
  if (isMenu)
  {
	  //If a menu play menu music and center camera
	  m_pRenderer->SetCameraPos(Vector3(m_vWinCenter.x, m_vWinCenter.y, m_pRenderer->GetCameraPos().z + offset));
	  m_pAudio->loop(underPar);
  }
  else
  {
	  //If a level play level music and load level
	  m_pAudio->loop(levelMusic);
	  m_pObjectManager->LoadMap(m_nCurLevel); //load map
  }

  CreateObjects(); //create new objects

  if (m_nCurLevel == -1)//For title screen, reset total strokes
  {
	  TotalStrokes = 0;
  }
  else if ((m_pPlayer->m_vPos.Distance(m_pPlayer->m_vPos, m_pBall->m_vPos)) <= 150)//If ball and player are near eachother, decrement stokes to account for initial loading hit
  {
	  TotalStrokes--;
	  Stroke = -1;
  }
  else //Otherwise, initialize course stroke to 0
  {
	  Stroke = 0;
  }
} //BeginGame

/// Poll the keyboard state and respond to the
/// key presses that happened since the last frame.

void CGame::KeyboardHandler(){
  m_pKeyboard->GetState(); //get current keyboard state 
  m_pController->GetState();

  if (m_pKeyboard->TriggerUp(VK_SPACE) || ((m_pController)->GetButtonAToggle()))
  {
	  if (isMenu) //Player cant move
	  {
		  //Begin the game or go to next level

		  //If not the final level, then dont make the next screen a menu (If final level, and retry, next screen is also not a menu) 
		  if ((m_nCurLevel != finalLevel) || (Retry))
		  {
			  isMenu = false;
		  }

		  if (m_nCurLevel == (finalLevel - 1))
		  {
			  m_pGoal->Deactivate();
		  }
		  
		  if (Retry)
		  {
			  BeginGame();
		  }
		  else if (m_nCurLevel == finalLevel)
		  {
			  m_nCurLevel = -1;
			  BeginGame();
		  }
		  else
		  {
			  NextLevel();
		  }
	  }
	  else
	  {
		  m_pAudio->play(SWINGCLUB);
	  }
  }
  if (m_pKeyboard->TriggerDown(VK_F1))
  {
		 m_bDrawAABBs = !m_bDrawAABBs;
  }
  if (m_pKeyboard->TriggerUp('R'))
  {
	  NextLevel();
	  
  }
  if (m_pKeyboard->TriggerDown('K'))
  {
	  shake = 4.0f;
  }
} //KeyboardHandler



/// Ask the object manager to draw the game objects. The renderer
/// is notified of the start and end of the frame so
/// that it can let Direct3D do its pipelining jiggery-pokery.

void CGame::RenderFrame(){
	
	if (isMenu)
	{
		m_pRenderer->BeginFrame();
		m_pObjectManager->draw();
		if (m_nCurLevel == finalLevel) //Win screen for final level
		{
			string EndingUI = "You beat the game in " + to_string(TotalStrokes) + " strokes";
			m_pRenderer->DrawScreenText(EndingUI.c_str(), Vector2(30.0f, 30.0f));

			if (TotalStrokes <= 0) //0 or fewer strokes, and you probably cheated
			{
				string JudgementUI = "Wow, you must have cheated!! >:(";
				m_pRenderer->DrawScreenText(JudgementUI.c_str(), Vector2(30.0f, 60.0f));
			}
			else if (TotalStrokes <= 50) // A good win
			{
				string JudgementUI = "Amazing, you're a golf star!";
				m_pRenderer->DrawScreenText(JudgementUI.c_str(), Vector2(30.0f, 60.0f));
			}
			else //You just beat the game, meh
			{
				string JudgementUI = "Eh, you could have done better.";
				m_pRenderer->DrawScreenText(JudgementUI.c_str(), Vector2(30.0f, 60.0f));
			}
		}
		m_pRenderer->EndFrame();
	}
	else
	{
		if (!canShake)
		{
			shake = approach(shake, 0, 0.1f);
		}
	
		shake == 0 ? canShake = true: canShake = false;

		m_vScreenShake.x = approach(((rand()%33) -16) *shake, 0, 0.1f);
		m_vScreenShake.y = approach(((rand()% 33) - 16)*shake, 0, 0.1f);
  		m_pRenderer->SetCameraPos(Vector3(m_pPlayer->m_vPos.x + m_vScreenShake.x, m_pPlayer->m_vPos.y + m_vScreenShake.y, m_pRenderer->GetCameraPos().z + offset));
		m_pRenderer->BeginFrame();
		
		//m_pRenderer->Draw(BACKGROUND_SPRITE, m_vWinCenter, 0); //draw background


		const Vector2 pos(m_pRenderer->GetCameraPos().x, m_pRenderer->GetCameraPos().y);

		m_pObjectManager->draw();
		if ((CBall*)m_pBall != nullptr)
		{

			if (((CBall*)m_pBall)->inRange)
			{
				CSpriteDesc2D desc;
				desc.m_nSpriteIndex = ARROW;
				desc.m_vPos = ((CBall*)m_pBall)->m_vPos;
				desc.m_fRoll = ((CBall*)m_pBall)->m_fPlayertoBallAngle - XM_PI;
				desc.m_fXScale = ((CBall*)m_pBall)->Charge;
				desc.m_fYScale = ((CBall*)m_pBall)->Charge;
				m_pRenderer->Draw(desc); //draw Arrow
			}
		}
		m_pParticleEngine->Draw(); //draw particles
		if (m_pPlayer != nullptr)
		{
			string PlayerUI = "Player Health: " + to_string(m_pPlayer->GetHP());
			m_pRenderer->DrawScreenText(PlayerUI.c_str(), Vector2(30.0f, 30.0f));

			string BallUI = "Ball Health: " + to_string(m_pBall->GetHP());
			m_pRenderer->DrawScreenText(BallUI.c_str(), Vector2(30.0f, 170.0f));

			string StrokeUI = "Strokes: " + to_string(Stroke) + " / " + to_string(CoursePar);
			m_pRenderer->DrawScreenText(StrokeUI.c_str(), Vector2(1300.0f, 60.0f));

			string StrokeUITot = "Total Strokes: " + to_string(TotalStrokes);
			m_pRenderer->DrawScreenText(StrokeUITot.c_str(), Vector2(1300.0f, 30.0f));
	
		}
		/* For tutorial levels, draw relevant screen text */
		if (m_nCurLevel == 0) //Tutorial 1 (Ball)
		{
			string TutorialUI = "Controls\n-------------\nMove: WASD or Left Analog\nAttack/HitBall: Space or A Button\nSprint: Shift or Left Trigger";
			m_pRenderer->DrawScreenText(TutorialUI.c_str(), Vector2(500.0f, 30.0f));

			string TutorialUI2 = "Objective\n---------------\nSafely escort the golf ball to\nthe hold at the end of\neach level and defeat the evil Dr. Bogey\nto escape the virtual world!\n\nEach course has a par that you must stay\nunder, for each enemy you kill with the ball\nyou get one stroke back.\n\nIf you or your ball lose all your\nhealth OR you go over par, GAME OVER!";
			m_pRenderer->DrawScreenText(TutorialUI2.c_str(), Vector2(30.0f, 200.0f));
		}
		else if(m_nCurLevel == 1) //Tutorial 2 (Fi Leone)
		{ 
			string TutorialUI = "Fi Leone\n-------------\nThe natural enemy of the bird, they'll rush\ntowards you and hurt you\nwith their tornado attack.\n\nHit them with your club or\nknock the golf ball into\nthem to kill them.";
			m_pRenderer->DrawScreenText(TutorialUI.c_str(), Vector2(30.0f, 200.0f));
		}
		else if (m_nCurLevel == 2) //Tutorial 3 (Bogey Bugs)
		{
			string TutorialUI = "Bogey Bugs\n---------------\nThey'll shoot balls of snot at you if\nyou get too close. They're in the air\nso you have to hit the golf ball\ninto them to kill them.";
			m_pRenderer->DrawScreenText(TutorialUI.c_str(), Vector2(500.0f, 600.0f));
		}
		else if (m_nCurLevel == 3) //Tutorial 4 (MY PRECIOUS CHILD)
		{
			string TutorialUI = "BABY-DOSER\nBulldozer Amalgamate Blended with Youngster\nDesigned for Optimal Salient Entity Removal\n---------------------------\nThey'll rush toward your golf ball and play\nwith it until it breaks, be careful not\nto let the ball out of your sight!";
			m_pRenderer->DrawScreenText(TutorialUI.c_str(), Vector2(30.0f, 200.0f));
		}
		Vector2 HealthOffset = Vector2(-m_vWinCenter.x + 64, m_vWinCenter.y - 94);
		Vector2 HealthBallOffset = Vector2(-m_vWinCenter.x + 64, m_vWinCenter.y - 144);

		m_pRenderer->DrawLine(PBar, m_pRenderer->GetCameraPos() + HealthOffset, m_pRenderer->GetCameraPos() + (HealthOffset + (Vector2(3.0f * m_pPlayer->GetHP(), 0.0f))));
		m_pRenderer->DrawLine(BBar, m_pRenderer->GetCameraPos() + HealthBallOffset, m_pRenderer->GetCameraPos() + (HealthBallOffset + (Vector2(3.0f * m_pBall->GetHP(), 0.0f))));

		m_pRenderer->EndFrame();
	}
} //RenderFrame

/// Handle keyboard input, move the game objects and render 
/// them in their new positions and orientations. Notify the 
/// audio player at the start of each frame so that it can 
/// prevent multiple copies of a sound from starting on the
/// same frame. Notify the timer of the start and end of the
/// frame so that it can calculate frame time. 

void CGame::ProcessFrame(){
	

  KeyboardHandler(); //handle keyboard input

  m_pAudio->BeginFrame(); //notify audio player that frame has begun
  m_pTimer->BeginFrame(); //notify timer that frame has begun

  RenderFrame(); //render a frame of animation

  if (isMenu)
  {
  }
  else
  {  
	if ((m_pPlayer->IsDead()) || (m_pBall->IsDead())) //Lose conditions
	{
		isMenu = true;
		Retry = true;

		//Create retry screen
		m_pRenderer->SetCameraPos(Vector3(m_vWinCenter.x, m_vWinCenter.y, m_pRenderer->GetCameraPos().z + offset));
		m_pObjectManager->create(LOSE, m_vWinCenter, OBJECT);
		m_pAudio->stop(underPar);
		m_pAudio->stop(levelMusic);
		m_pAudio->play(LOSELEVEL);
		
	}
	m_pObjectManager->move(m_pPlayer); //move all objects	

	if (m_pGoal->isActive()) //If goal is activated, end the level
	{
		isMenu = true;
		Retry = false;

		if (m_nCurLevel == finalLevel) //Win screen for final level
		{
			m_pAudio->stop(underPar);
			m_pAudio->stop(levelMusic);
			m_pAudio->play(WINGAME);
			m_pRenderer->SetCameraPos(Vector3(m_vWinCenter.x, m_vWinCenter.y, m_pRenderer->GetCameraPos().z + offset));
			m_pObjectManager->create(WIN, m_vWinCenter, OBJECT);
		}
		else //Next level screen for other levels
		{
			m_pAudio->stop(underPar);
			m_pAudio->stop(levelMusic);
			
			m_pRenderer->SetCameraPos(Vector3(m_vWinCenter.x, m_vWinCenter.y, m_pRenderer->GetCameraPos().z + offset));
			m_pObjectManager->create(NEXT, m_vWinCenter, OBJECT);
			if (Stroke <= 1)
			{
				m_pAudio->play(HOLEINONE);
			}
			else {
				m_pAudio->play(WINLEVEL);
			}
		}
	}
  }
  m_pParticleEngine->step(); //move particles in particle effects
  m_pTimer->EndFrame(); //notify timer that frame has ended
} //ProcessFrame


CObjectManager * CGame::GetObjManager()
{
	return  m_pObjectManager;
}

void CGame::createBaddies()
{
	CTileManager* m_pTileManager = m_pObjectManager->m_pTileManager;
	m_chMap = m_pTileManager->m_chMap;
	int m_nHeight = m_pTileManager->m_nHeight;
	int m_nWidth = m_pTileManager->m_nWidth;
	float m_fTileSize = m_pTileManager->m_fTileSize;

	CSpriteDesc2D desc;
	desc.m_nSpriteIndex = NULL_SPRTIE;

	const Vector2 vstart = Vector2::Zero;//m_vTileRadius + Vector2(0, m_fTileSize*(m_nHeight - 1)); //start position
	Vector2 pos = vstart; //current position

	for (int i = 0; i < m_nHeight; i++) {
		for (int j = 0; j < m_nWidth; j++) {
			desc.m_vPos = pos;

			switch (m_chMap[i][j]) {
			case 'L':
				m_pEnemy = (CEnemy*)m_pObjectManager->create(LeoneIdle, Vector2(pos.x, pos.y), LEONE);
				m_pEnemy->setTarget(m_pPlayer);
				break;
			case 'B':
				m_pEnemy = (CEnemy*)m_pObjectManager->create(BugIdle, Vector2(pos.x,pos.y), BUG);
				m_pEnemy->setTarget(m_pPlayer);
				break;
			case 'D': 
				m_pEnemy = (CEnemy*)m_pObjectManager->create(Testsprite, Vector2(pos.x, pos.y), DOZER);
				m_pEnemy->setTarget(m_pBall);
				break;
			case 'I':
				m_pEnemy = (CEnemy*)m_pObjectManager->create(Testsprite, Vector2(pos.x, pos.y), HEAD);
				m_pEnemy->setTarget(m_pPlayer);
				break;
			case 'H':
				m_pGoal = (CGoal*)m_pObjectManager->create(Testsprite, Vector2(pos.x, pos.y), GOAL);
				break;
			default:   break;
			} //switch

			//m_pRenderer->Draw(desc);
			pos.x += m_fTileSize; //left to right
		} //for

		pos.x = vstart.x;
		pos.y -= m_fTileSize; //top to bottom
	} //for
}

void CGame::createPlayer()
{
	CTileManager* m_pTileManager = m_pObjectManager->m_pTileManager;
	m_chMap = m_pTileManager->m_chMap;
	int m_nHeight = m_pTileManager->m_nHeight;
	int m_nWidth = m_pTileManager->m_nWidth;
	float m_fTileSize = m_pTileManager->m_fTileSize;

	CSpriteDesc2D desc;
	desc.m_nSpriteIndex = NULL_SPRTIE;

	const Vector2 vstart = Vector2::Zero;//m_vTileRadius + Vector2(0, m_fTileSize*(m_nHeight - 1)); //start position
	Vector2 pos = vstart; //current position

	for (int i = 0; i < m_nHeight; i++) {
		for (int j = 0; j < m_nWidth; j++) {
			desc.m_vPos = pos;

			switch (m_chMap[i][j]) {
			case 'P':
				m_pPlayer = (CPlayer*)m_pObjectManager->create(PenguinWalkDown, Vector2(pos.x, pos.y), PLAYER);
				CCommon::m_pPlayer = m_pPlayer;
				
				break;

			default:   break;
			} //switch

			//m_pRenderer->Draw(desc);
			pos.x += m_fTileSize; //left to right
		} //for

		pos.x = vstart.x;
		pos.y -= m_fTileSize; //top to bottom
	} //for
	pos = vstart;
	for (int i = 0; i < m_nHeight; i++) {
		for (int j = 0; j < m_nWidth; j++) {
			desc.m_vPos = pos;

			switch (m_chMap[i][j]) {
			case 'O':
				m_pBall = (CBall*)m_pObjectManager->create(GolfBall, Vector2(pos.x , pos.y ), BALL); //created in player constructor to allow for multiplayer maybe
				CCommon::m_pBall = m_pBall;
				break;

			default:   break;
			} //switch

			//m_pRenderer->Draw(desc);
			pos.x += m_fTileSize; //left to right
		} //for

		pos.x = vstart.x;
		pos.y -= m_fTileSize; //top to bottom
	} //for
}




