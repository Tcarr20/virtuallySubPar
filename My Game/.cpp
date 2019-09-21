#include "World.h"


World::World() {
}

World::World(CObjectManager* world_pObjectManager)
{
	//m_pObjectManager = world_pObjectManager;
}


World::~World()
{
}
//
//void World::buildWorld(int TilesHigh, int TilesWide)
//{
//	for (int tileLat = 0; tileLat < TilesWide; tileLat++) // Tile up and down
//	{
//		for (int tileLon = 0; tileLon < TilesHigh; tileLon++) // tile left and right
//		{
//			m_pObjectManager->create(WALL_MASK, Vector2(tileLat*TileSize, tileLon* TileSize));
//		}
//
//	}
//}
