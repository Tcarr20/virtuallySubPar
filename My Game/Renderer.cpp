/// \file Renderer.cpp
/// \brief Code for the renderer class CRenderer.

#include "Renderer.h"
#include "ComponentIncludes.h"

#include "DebugPrintf.h"
#include "Abort.h"

CRenderer::CRenderer():
  CSpriteRenderer(Batched2D){
} //constructor

/// Load the specific images needed for this game.
/// This is where eSpriteType values from GameDefines.h get
/// tied to the names of sprite tags in gamesettings.xml. Those
/// sprite tags contain the name of the corresponding image file.
/// If the image tag or the image file are missing, then
/// the game should abort from deeper in the Engine code,
/// leaving you with a dialog box that tells you what
/// went wrong.

void CRenderer::LoadImages(){  
  BeginResourceUpload();

  Load(BACKGROUND_SPRITE, "background"); 
  Load(TEXTWHEEL_SPRITE, "textwheel");
  Load(PIG_SPRITE, "pig");
  Load(HOLE, "Hole");
  Load(PenguinWalkDown, "PenguinWalkDown");
  Load(PLAYER_MASK, "PLAYER_MASK");
  Load(WALL_MASK, "WALL_MASK");
  Load(TILE_SPRITE, "tile");
  Load(PenguinWalkUp, "PenguinWalkUp");
  Load(PenguinWalkLeft, "PenguinWalkLeft");
  Load(PenguinWalkRight, "PenguinWalkRight");
  Load(PenguinIdle, "PenguinIdle");
  Load(PenguinIdleL, "PenguinIdleL");
  Load(HIT, "HIT");
  Load(Testsprite, "Testsprite");
  Load(GREENLINE_SPRITE, "greenline");
  Load(LeoneIdle, "LeoneIdle");
  Load(LeoneRun, "LeoneRun");
  Load(LeoneAttack, "LeoneAttack");
  Load(DozerUp, "DozerUp");
  Load(DozerDown, "DozerDown");
  Load(DozerRight, "DozerRight");
  Load(DozerLeft, "DozerLeft");
  Load(BugIdle, "BugIdle");
  Load(BugAttack, "BugAttack");
  Load(BugProject, "SnotBall");
  Load(GolfBall, "GolfBall");
  Load(ARROW, "ARROW");
  Load(NULL_SPRTIE, "NullSprite");
  Load(TITLE, "subtitle");
  Load(NEXT, "subnext");
  Load(LOSE, "sublose");
  Load(WIN, "subwin");
  Load(DUST_SPRITE, "Dust");
  Load(HEAD_SPRITE, "Head");
  Load(HANDL_SPRITE, "HandLeft");
  Load(HANDR_SPRITE, "HandRight");
  Load(HEADHURT, "HeadHurt");
  Load(FGRADE, "FGrade");
  Load(PBar, "PlayerBar");
  Load(BBar, "BallBar");



  EndResourceUpload();
} //LoadImages



/// Reader function for camera position.
/// \return Camera position.

const Vector3& CRenderer::GetCameraPos() {
	return m_pCamera->GetPos();
} //GetCameraPos

/// Writer function for camera position.
/// \param pos New camera position.

void CRenderer::SetCameraPos(const Vector3& pos) {
	m_pCamera->MoveTo(pos);
} //SetCameraPos

/// Reader function for toggle camera status.
/// \return true if camera is a follow camera.

bool CRenderer::IsFollowCamera() {
	return m_bFollowCamera;
} //IsFollowCamera

/// Toggle follow camera status.
/// \param b true if camera is to become a follow camera.

void CRenderer::SetFollowCamera(bool b) {
	m_bFollowCamera = b;
} //SetFollowCamera

void CRenderer::SetCameraPersective(float width, float ht, float nearz, float farz) {
	m_pCamera->SetOrthographic(width, ht, nearz, farz);
} ///< Set camera perspective.

void CRenderer::DrawBoundingBox(const BoundingBox& aabb) {
	const Vector3 center = aabb.Center;
	const Vector3 extent = aabb.Extents;

	const Vector2 topleft(center.x - extent.x, center.y + extent.y);
	const Vector2 topright(center.x + extent.x, center.y + extent.y);
	const Vector2 bottomleft(center.x - extent.x, center.y - extent.y);
	const Vector2 bottomright(center.x + extent.x, center.y - extent.y);

	DrawLine(GREENLINE_SPRITE, topleft, topright); //top edge
	DrawLine(GREENLINE_SPRITE, bottomleft, bottomright); //bottom edge
	DrawLine(GREENLINE_SPRITE, topleft, bottomleft); //left edge
	DrawLine(GREENLINE_SPRITE, topright, bottomright); //right edge
} //DrawBoundingBox


