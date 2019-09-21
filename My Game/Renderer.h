/// \file Renderer.h 
/// \brief Interface for the renderer class CRenderer.

#pragma once

#include "GameDefines.h"
#include "SpriteRenderer.h"

/// \brief The renderer.
///
/// CRenderer handles the game-specific rendering tasks, relying on
/// the base class to do all of the actual API-specific rendering.

class CRenderer: public CSpriteRenderer{
  public:
    CRenderer(); ///< Constructor.
    void LoadImages(); ///< Load images.
	bool m_bFollowCamera = true; /// set camera to follow?

	const Vector3& GetCameraPos(); ///< Get camera position.
	void SetCameraPos(const Vector3& pos); ///< Set camera position.
	void SetCameraPersective(float width, float ht, float nearz, float farz); ///< Set camera perspective.

	bool IsFollowCamera(); ///< Test if follow camera.
	void SetFollowCamera(bool b = true); ///< Set/unset follow camera.
	void DrawBoundingBox(const BoundingBox& aabb); ///< Draw AABB.
}; //CRenderer
