

#pragma once

#include "Component.h"
#include "Common.h"
#include "CEnemy.h"
#include "ObjectManager.h"
#include "Settings.h"

class World
{
public:
	World();
	~World();
	int TileSize = 32;
	int WorldTileHigh = 120 ;
	int WorldTileWide = 120 ;
	bool DrawGrid = false;

	//void buildWorld(int TilesHigh, int TilesWide);
	//CObjectManager* m_pObjectManager;
	World(CObjectManager* world_pObjectManager);
	
};

