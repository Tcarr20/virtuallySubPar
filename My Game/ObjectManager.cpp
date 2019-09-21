/// \file ObjectManager.cpp
/// \brief Code for the the object manager class CObjectManager.

#include "ObjectManager.h"
#include "ComponentIncludes.h"
#include "ParticleEngine.h"

#include "DebugPrintf.h"

/// Create the tile manager;

CObjectManager::CObjectManager(){
	m_pTileManager = new CTileManager(32);
} //constructor

/// Destruct all of the objects in the object list.

CObjectManager::~CObjectManager(){
	delete m_pTileManager;
  for(auto const& p: m_stdObjectList)//for each object
    delete p; //delete object
	m_stdObjDrawVec.clear(); // clear the object drawing vector;
} //destructor

/// Create an object and put a pointer to it on the object list.
/// \param t Sprite type.
/// \param v Initial position.
/// \return Pointer to the object created.

CObject* CObjectManager::create(eSpriteType t, const Vector2& pos, objectTags Ta){
	CObject* p = nullptr;
					
	switch (Ta) {											/// used to create the correct child of object, else create object
	case PLAYER: p = new CPlayer(pos); break;		/// create the player object
	case LEONE: p = new CLeone(pos, 1, 10, 2, Ta); break; //Creates a Fi Leone
	case DOZER: p = new CDozer(pos, 3, 5, 1, Ta); break; //Create a BABY DOZER
	case BUG: p = new CBugs(pos, 1, 15, 0, Ta); break; //Create a Bogey Bug
	case BALL: p = new CBall(pos); break; // create the ball
	case EPROJECT: p = new CEnemyProjectile(pos, 15, 5.0f, Ta,t); break; //Create projectile
	case GOAL: p = new CGoal(pos, Ta); break; //Create goal post
	case HANDL: p = new CHand(pos, 10, 10, 2, Ta, HANDL_SPRITE); break; //Creates a HAND
	case HANDR: p = new CHand(pos, 10, 10, 2, Ta, HANDR_SPRITE); break; //Creates a HAND
	case HEAD: p = new CHead(pos, 10, 15, 0, Ta); break; //Create a HEAD
	default: p = new CObject(t, pos, Ta); break;				/// defulat create regular object
	} //switch
  m_stdObjectList.push_back(p); // add to list of all objects 
  m_stdObjDrawVec.push_back(p); // add to list of drawable objects
  return p;
} //create

/// Delete all of the objects managed by the object manager. 
/// This involves deleting all of the CObject instances pointed
/// to by the object list, then clearing the object list itself.



void CObjectManager::clear(){
	for (auto const& p : m_stdObjectList) { //for each object
		delete p; //delete object
	}
  m_stdObjectList.clear(); //clear the object list
  m_stdObjDrawVec.clear();
} //clear

/// Draw the tiled background and all of the objects.

bool helpSort(CObject *r, CObject *l) { return (r->depth > l->depth); } // this function is used for the sorting of drawable objects 

void CObjectManager::draw(){
	m_pTileManager->Draw(TILE_SPRITE);
	if (m_bDrawAABBs) {
		//m_pTileManager->DrawBoundingBoxes(GREENLINE_SPRITE);
	}

	stable_sort(m_stdObjDrawVec.begin(), m_stdObjDrawVec.end(),helpSort); // this applies the depth variable in objects to the draw order

	for (auto const& p : m_stdObjDrawVec) { //for each drawable object
		m_pRenderer->Draw(*(CSpriteDesc2D*)p); // draw it
		if (m_bDrawAABBs) {
		m_pRenderer->DrawBoundingBox(p->GetBoundingBox());
		//m_pRenderer->DrawBoundingBox(((CBall*)p)->m_AabbxVel);// don't draw this, it has a memory leak in debug mode...
		//m_pRenderer->DrawBoundingBox(((CBall*)p)->m_AabbyVel);// or this
		}//if

	} //for
} //draw

/// Move all of the objects and perform 
/// broad phase collision detection and response.

void CObjectManager::move(CObject* Player){

	for (auto const& p : m_stdObjectList) //for each object
	{
		switch (p->Tag) //Determine tag of the object
		{
		case PLAYER: //If the object is a player
			p->Move();
			break;
		case LEONE: //If the object is Fi Leone
			if (Player != nullptr) {
				((CLeone*)p)->ProcessState();
				((CLeone*)p)->Move();
			}
			break;
		case DOZER: //If the object is BABY DOZER
			if (Player != nullptr)
			{
				//REPLACE PLAYER POINTER WITH BALL POINTER LATER
				((CDozer*)p)->ProcessState();
				((CDozer*)p)->Move();
			}
			p->Move();
			break;
		case BUG: //If the object is a Bogey Bug 
			if (Player != nullptr){
				((CBugs*)p)->ProcessState();
				((CBugs*)p)->Move();
			}
		case BALL: //If the object is a Bogey Bug 
			if (Player != nullptr) {
				((CBall*)p)->Move();
			}
			break;
		case HEAD: //If the object is a Bogey Bug 
			if (Player != nullptr) {
				((CHead*)p)->ProcessState();
				((CHead*)p)->Move();
			}
			break;
		case HANDL: //If the object is a Bogey Bug 
			if (Player != nullptr) {
				((CHand*)p)->ProcessState();
				((CHand*)p)->Move();
			}
			break;
		case HANDR: //If the object is a Bogey Bug 
			if (Player != nullptr) {
				((CHand*)p)->ProcessState();
				((CHand*)p)->Move();
			}
			break;
		case EPROJECT:
			if(Player != nullptr){
				((CEnemyProjectile*)p)->Move();
			}
			break;
		case GOAL:
			if (Player != nullptr)
			{
				((CGoal*)p)->Move();
			}
			break;
		default: //Default case
			break;
		}
	}

	if (m_pKeyboard->TriggerDown(VK_F2))
	{
		
	}

  BroadPhase(); //broad phase collision detection and response
  CullDeadObjects(); //remove dead objects from object list
} //move

/// This is a "bring out yer dead" Monty Python type of thing.
/// Iterate through the objects and check whether their "is dead"
/// flag has been set. If so, then delete its pointer from
/// the object list and destruct the object.

void CObjectManager::CullDeadObjects(){
  for(auto i=m_stdObjectList.begin(); i!=m_stdObjectList.end();){
    if((*i)->IsDead()){ //"He's dead, Dave." --- Holly, Red Dwarf
      i = m_stdObjectList.erase(i); //remove from object list and advance to next object
    } //if
      
    else ++i; //advance to next object
  } //for

  for (auto i = m_stdObjDrawVec.begin(); i != m_stdObjDrawVec.end();) {
	  if ((*i)->IsDead()) { //"He's dead, Dave." --- Holly, Red Dwarf
		  delete *i; //delete object
		  i = m_stdObjDrawVec.erase(i); //remove from object list and advance to next object
	  } //if

	  else ++i; //advance to next object
  } //for
} //CullDeadObjects

/// Perform collision detection and response for all pairs
/// of objects in the object list, making sure that each
/// pair is processed only once.

void CObjectManager::BroadPhase(){
  for(auto i=m_stdObjectList.begin(); i!=m_stdObjectList.end(); i++){
    for(auto j=next(i); j!=m_stdObjectList.end(); j++)
      NarrowPhase(*i, *j);
  } //for
} //BroadPhase

/// Perform collision detection and response for a pair of objects.
/// \param p0 Pointer to the first object.
/// \param p1 Pointer to the second object.

void CObjectManager::NarrowPhase(CObject* p0, CObject* p1){
	
	if ((((CPlayer*)p0)->m_AabbxVel.Intersects(p1->m_Aabb)) ||
		(((CPlayer*)p0)->m_AabbyVel.Intersects(p1->m_Aabb))) { //bounding box intersect
		if ((p0->Tag == PLAYER) && p1->Tag == OBJECT) { //player hits object
			((CPlayer*)p0)->CollisionResponse(p1);
		}
		else if (((p0->Tag == PLAYER) && p1->Tag == BALL)) {
			//((CBall*)p1)->inRange = true;

		}
		else if (((p0->Tag == PLAYER) && p1->Tag == EPROJECT)) {
			((CPlayer*)p0)->TakeDamage(15);
			((CEnemyProjectile*)p1)->kill();
		}
	}
	if ((((CPlayer*)p1)->m_AabbxVel.Intersects(p0->m_Aabb)) ||
		(((CPlayer*)p1)->m_AabbyVel.Intersects(p0->m_Aabb))) { //bounding box intersect
		if ((p1->Tag == PLAYER) && p0->Tag == OBJECT) { //player hits object
			((CPlayer*)p1)->CollisionResponse(p0);

		}
		else if(((p1->Tag == PLAYER) && p0->Tag == BALL)) {
			//((CBall*)p0)->inRange = true;
			
		}
		else if (((p1->Tag == PLAYER) && p0->Tag == EPROJECT)) {
			((CPlayer*)p1)->TakeDamage(15);
			((CEnemyProjectile*)p0)->kill();
		}
	}


} //NarrowPhase

//Load the map from a text file!
//param level takes an integer representing the the level number

void CObjectManager::LoadMap(int level) {
	char tutone[50] = "Media\\Maps\\Tut1.txt";
	char tuttwo[50] = "Media\\Maps\\Tut2.txt";
	char tutthree[50] = "Media\\Maps\\Tut3.txt";
	char tutfour[50] = "Media\\Maps\\Tut4.txt";
	char one[50] = "Media\\Maps\\Lv1.txt";
	char two[50] = "Media\\Maps\\Lv2.txt";
	char three[50] = "Media\\Maps\\Lv3.txt";
	char four[50] = "Media\\Maps\\Lv4.txt";
	char five[50] = "Media\\Maps\\Lv5.txt";
	char six[50] = "Media\\Maps\\Lv6.txt";
	char seven[50] = "Media\\Maps\\Lv7.txt";
	char eight[50] = "Media\\Maps\\Lv8.txt";
	char nine[50] = "Media\\Maps\\Lv9.txt";
	char ten[50] = "Media\\Maps\\boss.txt";
	switch (level) {
		case 0: m_pTileManager->LoadMap(tutone); CoursePar = 10;  break;
		case 1: m_pTileManager->LoadMap(tuttwo); CoursePar = 10;  break;
		case 2: m_pTileManager->LoadMap(tutthree); CoursePar = 10; break;
		case 3: m_pTileManager->LoadMap(tutfour); CoursePar = 10;  break;
		case 4: m_pTileManager->LoadMap(one); CoursePar = 7; break;
		case 5: m_pTileManager->LoadMap(two); CoursePar = 7; break;
		case 6: m_pTileManager->LoadMap(three); CoursePar = 8;  break;
		case 7: m_pTileManager->LoadMap(four); CoursePar = 10; break;
		case 8: m_pTileManager->LoadMap(five); CoursePar = 10; break;
		case 9: m_pTileManager->LoadMap(six); CoursePar = 10; break;
		case 10: m_pTileManager->LoadMap(seven); CoursePar = 15; break;
		case 11: m_pTileManager->LoadMap(eight); CoursePar = 20; break;
		case 12: m_pTileManager->LoadMap(nine); CoursePar = 30; break;
		case 13: m_pTileManager->LoadMap(ten); CoursePar = 100; break;
		
	} //switch
} //LoadMap

void CObjectManager::GenMaps(int level) {

	char one[50] = "Media\\Maps\\test.txt";
	char two[50] = "Media\\Maps\\test.txt";
	char three[50] = "Media\\Maps\\test.txt";
	switch (level) {

	case 0: m_pTileManager->GenMaps(one); break;
	case 1: m_pTileManager->GenMaps(two); break;
	case 2: m_pTileManager->GenMaps(three); break;
	} //switch

}//GenMaps


