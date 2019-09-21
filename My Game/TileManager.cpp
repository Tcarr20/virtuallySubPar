/// \file TileManager.cpp
/// \brief Code for the tile manager CTileManager.

#include "TileManager.h"
#include "Renderer.h"

#include "DebugPrintf.h"
#include "Abort.h"

/// \param tilesize Width and height of square tile in pixels.

CTileManager::CTileManager(size_t tilesize) :
	m_fTileSize((float)tilesize),
	m_vTileRadius(Vector2(m_fTileSize, m_fTileSize) / 2) {
} //constructor

/// Delete the memory used for storing the map.

CTileManager::~CTileManager() {
	for (int i = 0; i < m_nHeight; i++)
		delete[] m_chMap[i];

	delete[] m_chMap;
} //destructor

/// Make the AABBs for the walls. Care is taken to use the
/// longest horizontal and vertical AABBs possible so that
/// there aren't so many of them.

void CTileManager::MakeBoundingBoxes() {
	bool** used = new bool*[m_nHeight]; //which walls have been used in an AABB already

	for (int i = 0; i < m_nHeight; i++) {
		used[i] = new bool[m_nWidth];

		for (int j = 0; j < m_nWidth; j++)
			used[i][j] = false;
	} //for

	m_vecWalls.clear(); //no walls yet

	BoundingBox aabb; //current bounding box
	const Vector3 vTileExtents = 0.5f*Vector3(m_fTileSize, m_fTileSize, m_fTileSize);

	//horizontal walls with more than one tile

	const Vector2 vstart = Vector2::Zero;//m_vTileRadius + Vector2(0, m_fTileSize*(m_nHeight - 1)); //start position
	Vector2 pos = vstart; //current position

	for (int i = 0; i < m_nHeight; i++) {
		int j = 0;
		pos.x = vstart.x;

		while (j < m_nWidth) {
			while (j < m_nWidth && m_chMap[i][j] != 'W') {
				j++; pos.x += m_fTileSize;
			} //while

			if (j < m_nWidth) {
				aabb.Center = Vector3(pos.x, pos.y, 0);
				aabb.Extents = vTileExtents;
				used[i][j] = true;
				j++; pos.x += m_fTileSize;
			} //if

			bool bSingleTile = true;

			while (j < m_nWidth && m_chMap[i][j] == 'W') {
				BoundingBox b;
				b.Center = Vector3(pos.x, pos.y, 0);
				b.Extents = vTileExtents;
				BoundingBox::CreateMerged(aabb, aabb, b);
				used[i][j] = true;
				bSingleTile = false;
				j++; pos.x += m_fTileSize;
			} //while

			if (!bSingleTile)
				m_vecWalls.push_back(aabb);
		} //while

		pos.y -= m_fTileSize;
	} //for

	//vertical walls, the single tiles get caught here

	pos = vstart; //current position

	for (int j = 0; j < m_nWidth; j++) {
		int i = 0;
		pos.y = vstart.y;

		while (i < m_nHeight) {
			while (i < m_nHeight && m_chMap[i][j] != 'W') {
				i++; pos.y -= m_fTileSize;
			} //while

			if (i < m_nHeight) {
				aabb.Center = Vector3(pos.x, pos.y, 0);
				aabb.Extents = vTileExtents;
				used[i][j] = true;
				i++; pos.y -= m_fTileSize;
			} //if

			bool bSingleTile = true;

			while (i < m_nHeight && m_chMap[i][j] == 'W') {
				BoundingBox b;
				b.Center = Vector3(pos.x, pos.y, 0);
				b.Extents = vTileExtents;
				BoundingBox::CreateMerged(aabb, aabb, b);
				used[i][j] = true;
				bSingleTile = false;
				i++; pos.y -= m_fTileSize;
			} //while

			if (!bSingleTile)
				m_vecWalls.push_back(aabb);
		} //while

		pos.x += m_fTileSize;
	} //for

	//orphaned single tiles that are not on an edge

	pos = vstart + Vector2(m_fTileSize, -m_fTileSize);

	for (int i = 1; i < m_nHeight - 1; i++) {
		for (int j = 1; j < m_nWidth - 1; j++) {
			if (m_chMap[i][j] == 'W' &&
				(m_chMap[i - 1][j] != 'W' && m_chMap[i + 1][j] != 'W' &&
					m_chMap[i][j - 1] != 'W' && m_chMap[i][j + 1] != 'W'))
			{
				BoundingBox b;
				b.Center = Vector3(pos.x, pos.y, 0);
				b.Extents = vTileExtents;
				m_vecWalls.push_back(b);
			} //if

			pos.x += m_fTileSize;
		} //for

		pos.x = vstart.x + m_fTileSize;
		pos.y -= m_fTileSize;
	} //for

	//clean up and exit

	for (int i = 0; i < m_nHeight; i++)
		delete[] used[i];

	delete[] used;
} //MakeBoundingBoxes

/// Delete the old map (if any), allocate the right sized
/// chunk of memory for the new map, and read it from a
/// text file.
/// \param filename Name of the map file.

void CTileManager::LoadMap(char* filename) {
	if (m_chMap != nullptr) { //unload any previous maps
		for (int i = 0; i < m_nHeight; i++)
			delete[] m_chMap[i];

		delete[] m_chMap;
	} //if

	FILE *input; //input file handle

	fopen_s(&input, filename, "rb"); //open the map file
	if (input == nullptr) //abort if it's missing
		ABORT("Map &s not found.", filename);

	//read map file into a character buffer 

	fseek(input, 0, SEEK_END); //seek to end of map file
	const int n = ftell(input); //get file size in bytes
	rewind(input); //seek to start of file

	char *buffer = new char[n + 1]; //temporary character buffer
	fread(buffer, n, 1, input); //read the whole thing in a chunk
	fclose(input); //close the map file, we're done with it

	//get map width and height into m_nWidth and m_nHeight

	m_nWidth = -1; m_nHeight = 0;
	int w = 0; //width of current row

	for (int i = 0; i < n; i++) {
		if (buffer[i] != '\n')
			w++; //skip characters until the end of line
		else {
			if (w == 0)ABORT("Panic!");
			if (w != m_nWidth && m_nWidth >= 0 && w != 0) //not the same length as the previous one
				ABORT("Line %d of map is not the same length as the previous one.", m_nHeight);
			m_nWidth = w; w = 0; m_nHeight++; //next line
		} //else
	} //for

	m_nWidth--;

	//allocate space for the map 

	m_chMap = new char*[m_nHeight];

	for (int i = 0; i < m_nHeight; i++)
		m_chMap[i] = new char[m_nWidth];

	//load the map information from the buffer to the map

	int index = 0; //index into character buffer

	for (int i = 0; i < m_nHeight; i++) {
		for (int j = 0; j < m_nWidth; j++) {
			m_chMap[i][j] = buffer[index]; //load character into map
			index++; //next index
		} //for
		index += 2; //skip end of line character
	} //for

	m_vWorldSize = Vector2((float)m_nWidth, (float)m_nHeight)*m_fTileSize;
	MakeBoundingBoxes();

	delete[] buffer; //clean up
} //LoadMap

/// This is for debug purposes so that you can verify that
/// the collision shapes are in the right places.
/// \param t Line sprite to be stretched to draw the line.

void CTileManager::DrawBoundingBoxes(eSpriteType t) {
	for (auto& p : m_vecWalls)
		m_pRenderer->DrawBoundingBox(p);
} //DrawBoundingBoxes

/// Draw order is top-down, left-to-right so that the image
/// agrees with the map text file viewed in NotePad.
/// \param t Sprite type for a 3-frame sprite: 0 is floor, 1 is wall, 2 is an error flag.

void CTileManager::Draw(eSpriteType t) {
	CSpriteDesc2D desc;
	desc.m_nSpriteIndex = t;

	const Vector2 vstart = Vector2::Zero;//m_vTileRadius + Vector2(0, m_fTileSize*(m_nHeight - 1)); //start position
	Vector2 pos = vstart; //current position

	for (int i = 0; i < m_nHeight; i++) {
		for (int j = 0; j < m_nWidth; j++) {
			desc.m_vPos = pos;

			switch (m_chMap[i][j]) {
			case 'F': desc.m_nCurrentFrame = 0; break;
			case 'W': desc.m_nCurrentFrame = 1; break; 
			default:  desc.m_nCurrentFrame = 0; break;
			} //switch

			m_pRenderer->Draw(desc);
			pos.x += m_fTileSize; //left to right
		} //for

		pos.x = vstart.x;
		pos.y -= m_fTileSize; //top to bottom
	} //for
} //Draw

/// Check whether a circle is visible from a point, that is, either the left
/// or the right side of the object (from the perspective of the point)
/// has no walls between it and the point. This gives some weird behavior
/// when the circle is partially hidden by a block, but it doesn't seem
/// particularly unnatural in practice. It'll do.
/// \param p0 A point.
/// \param p1 Center of circle.
/// \param radius Radius of circle.
/// \return true If the circle is visible from the point.

bool CTileManager::Visible(const Vector2& p0, const Vector2& p1, float radius) {
	bool visible = true;

	for (auto i = m_vecWalls.begin(); i != m_vecWalls.end() && visible; i++) {
		Vector2 direction = p0 - p1;
		direction.Normalize();
		const Vector2 norm = Vector2(-direction.y, direction.x);

		const float delta = min(radius, 16.0f);

		//left-hand triangle
		const Vector3 v0(p0);
		const Vector3 v1(p1 + radius * norm);
		const Vector3 v2(p1 + (radius - delta)*norm);

		//right-hand triangle (or perhaps it's the other way round)
		const Vector3 v3(p1 - radius * norm);
		const Vector3 v4(p1 - (radius - delta)*norm);

		visible = !(*i).Intersects(v0, v1, v2) || !(*i).Intersects(v0, v3, v4);
	} //for

	return visible;
} //Visible


void CTileManager::GenMaps(char* filename) {

	FILE *input; //input file handle


	fopen_s(&input, filename, "w"); //open the map file
	if (input == nullptr) //abort if it's missing
		ABORT("Map &s not found.", filename);

	m_nHeight = 32;
	m_nWidth = 32;

	char buff[] = {' '}; // initialsize the character array, becuase that's how you have to send it for fwrite function to work
	for (int h = 0; h <= m_nHeight; h++)
	{
		for (int w = 0; w <= m_nWidth ; w++)
		{
			if ((h == 0) || h == m_nHeight || w == 0 || w == m_nWidth)
			{
				buff[0] = 'W'; // wall tile
				fwrite(buff, sizeof(buff), 1, input);
			}
			else {
				buff[0] = 'F';// floor tile
				fwrite(buff, sizeof(buff), 1, input);
			}
		}
		buff[0] = '\n';// end line
		fwrite(buff, sizeof(buff), 1, input); // write to the file 
	}
	

	

	fclose(input); //close the map file, we're done with it
	//fopen_s(&input, filename, "wr"); //open the map file
	//ChangeTile(input, 1, 1, 'W'); // change the tile
	//fclose(input);

} //GenMap

void CTileManager::ChangeTile(FILE* input, int height, int width, char tile)
{

	//FILE *input; //input file handle

	//fopen_s(&input, fileName, "r"); //open the map file for reading
	if (input == nullptr) //abort if it's missing
		ABORT("Map not found.");

	//read map file into a character buffer 

	fseek(input, 0, SEEK_END); //seek to end of map file
	const int n = ftell(input); //get file size in bytes
	rewind(input); //seek to start of file

	char *buffer = new char[n + 1]; //temporary character buffer
	fread(buffer, n, 1, input); //read the whole thing in a chunk
	//fclose(input); //close the map file, we're done with it

	//fopen_s(&input,fileName, "w"); //open the map file for wirting
	//buffer[0] = 'W';// floor tile
	fwrite(buffer, sizeof(buffer), n, input);
	//fclose(input);
	

}

