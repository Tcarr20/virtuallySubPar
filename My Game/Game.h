/// \file Game.h
/// \brief Interface for the game class CGame.

#pragma once

#include "Component.h"
#include "Common.h"
#include "CEnemy.h"
#include "ObjectManager.h"
#include "Settings.h"
#include "TileManager.h"
#include "CBall.h"
#include "CEnemyProjectile.h"
#include "CGoal.h"


/// \brief The game class.

class CGame: 
  public CComponent, 
  public CSettings,
  public CCommon{ 

  private:
	  int m_nCurLevel = -1; ///< Current level number.

	std::unique_ptr<DirectX::Mouse> m_mouse;
	void NextLevel();
	void BeginGame(); ///< Begin playing the game.
    void KeyboardHandler(); ///< The keyboard handler.
    void RenderFrame(); ///< Render an animation frame.
    void CreateObjects(); ///< Create game objects.
	int finalLevel = 13;
	bool isMenu = true; //True when the player has no control (i.e. on a menu) | False when the player can move (i.e. Not a menu)
	bool Retry = false; //True if either the ball of player is dead

  public:
	  CPlayer* m_pPlayer = nullptr; //Pointer to the player
	  CEnemy* m_pEnemy = nullptr; //Pointer to an enemy
	  CBall* m_pBall = nullptr;  //Pointer to a ball
	  //CGoal* m_pGoal = nullptr; //Pointer to the goal
	  CObject* m_DefaultObj = nullptr; //Default object pointer, really used for title screen
    ~CGame(); ///< Destructor.

    void Initialize(); ///< Initialize the game.
    void ProcessFrame(); ///< Process an animation frame.
	bool canShake = true;
	int TileSize = 32;

	CObjectManager* GetObjManager();

	void createBaddies();

	void createPlayer();

	float offset = 0;//DEBUG

	char** m_chMap = nullptr; ///< The level map.
}; //CGame


