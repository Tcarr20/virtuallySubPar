/// \file GameDefines.h
/// \brief Game specific defines.

#pragma once

#include "Defines.h"

/// \brief Sprite type.
///
/// Note: NUM_SPRITES must be last.

enum eSpriteType{
  BACKGROUND_SPRITE, TEXTWHEEL_SPRITE, Testsprite, PIG_SPRITE, PLAYER_MASK, WALL_MASK, TILE_SPRITE,ARROW, NULL_SPRTIE, TITLE, NEXT, LOSE, WIN, HOLE,HIT, DUST_SPRITE,
  PenguinWalkDown, PenguinWalkUp, PenguinWalkLeft,PenguinWalkRight, PenguinIdle, PenguinIdleL, 
  GREENLINE_SPRITE, HEAD_SPRITE, HANDL_SPRITE, HANDR_SPRITE,HEADHURT, FGRADE, PBar, BBar,
  LeoneIdle, LeoneRun, LeoneAttack, //Fi Leone sprites 
  DozerUp, DozerDown, DozerLeft, DozerRight, //Baby Dozer sprites 
  BugIdle, BugAttack, BugProject, //Bogey Bug Sprites
  GolfBall, //Other sprites
  NUM_SPRITES //MUST BE LAST
}; //eSpriteType

//Type for different types of objects in the game.
enum objectTags
{
	OBJECT, PLAYER, LEONE, DOZER, BUG, BALL, EPROJECT, GOAL, HANDL,HANDR,HEAD
}; 
