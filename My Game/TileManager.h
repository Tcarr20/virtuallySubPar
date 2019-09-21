/// \file TileManager.h
/// \brief Interface for the tile manager CTileManager.

#pragma once

#include <vector>

using namespace std;

#include "Common.h"
#include "Sprite.h"
#include "GameDefines.h"

/// \brief The tile manager.
///
/// The tile manager is responsible for the
/// tile-based background.

class CTileManager : public CCommon {
private:


	Vector2 m_vTileRadius; ///< Tile radius.

	vector<BoundingBox> m_vecWalls; ///< AABBs for the walls.

	void MakeBoundingBoxes(); ///< Make bounding boxes for walls.

public:
	CTileManager(size_t tilesize); ///< Constructor.
	~CTileManager(); ///< Destructor.

	void LoadMap(char* filename); ///< Load a map.
	void Draw(eSpriteType t); ///< Draw the map with a given tile.
	void DrawBoundingBoxes(eSpriteType t); ///< Draw the bounding boxes.

	bool Visible(const Vector2& v0, const Vector2& v1, float radius); ///< Check visibility.

	void GenMaps(char * filename);

	void ChangeTile(FILE* input, int height, int width, char tile);

	template<class t> bool CollideWithWall(const t& s); ///< Check object collision with a wall.

	char** m_chMap = nullptr; ///< The level map.
	int m_nWidth = 0; ///< Number of tiles wide.
	int m_nHeight = 0; ///< Number of tiles high.
	float m_fTileSize; ///< Tile width and height.
}; //CTileManager

/// \brief Template for bounding shape collisions.
/// Determine whether a bounding shape intersects a wall.
/// \param s A bounding shape.
/// \return true if the bounding shape intersects a wall.

template<class t> bool CTileManager::CollideWithWall(const t& s) {
	bool hit = false;

	for (auto i = m_vecWalls.begin(); i != m_vecWalls.end() && !hit; i++)
		hit = s.Intersects(*i);

	return hit;
} //CollideWithWall