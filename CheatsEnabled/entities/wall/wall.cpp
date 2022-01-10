// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module:			Gameplay Programming
// Assignment1:		Cheats:Enabled
// Student Name:	Eugene Long
// Student No.:		S10193060J
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// ===========================================================================
// InkdotEngine - Wall Entity Class Implementation
// ===========================================================================

// include specification
#include "wall.h"

//============================================================================
// default constructor
//============================================================================
Wall::Wall() :

	// base object constructor
	InkdotEntity(),

	// entity states
	topLeftX	(),
	topLeftY	(),
	width		(),
	height		()
{}

//============================================================================
// default destructor
//============================================================================
Wall::~Wall() {}


// game logic methods

// entity methods

//============================================================================
// initializes the entity - called on scene initialization
//============================================================================
void Wall::initialize(
	float x,
	float y,
	float xEdge,
	float yEdge
) {
	// initialize base object
	InkdotEntity::initialize(x, y, 0, 0);

	// add collider
	RECT colliderRect = {
		long(-xEdge), 
		long(-yEdge), 
		long(xEdge), 
		long(yEdge) 
	};
	addCollider("sprite", 0, 0, colliderRect);

	// calculate states for primitive rect drawing - walls don't need to move,
	// so we don't need to recalculate this much.
	topLeftX	= x - xEdge;
	topLeftY	= y - yEdge;
	width		= xEdge * 2;
	height		= yEdge * 2;

	// add wall tag to wall - check for this to collide against walls
	addTag("WALL");
}

//============================================================================
// renders entity sprites - called after background sprites are drawn
//============================================================================
void Wall::renderSprites()
{
	// draw primitive rect at wall coordinates every render
	getGraphics()->drawPrimitiveRect(
		topLeftX,
		topLeftY,
		128,
		width,
		height,
		CommonColors::BLACK
	);
}

//============================================================================
// updates the entity - called each frame before scene update() starts
//============================================================================
void Wall::onUpdateEnd(
	float	prevFrameTime
) {

	// apply entity velocity to its position
	applyVelocity(prevFrameTime);

	//if (this->collidesWithTaggedEntities(
	//	"sprite",
	//	"player",
	//	collisionList
	//)) {

	//	InkdotEntity* e;

	//	// handle each collision in collision list
	//	for (size_t i = 0; i < collisionList.size(); i++) {

	//		e = entityManager->getEntity(std::get<2>(collisionList[i]));
	//		if (std::get<0>(collisionList[i]).y > 0) {
	//			e->setVelocityY(0);
	//		}
	//		if (std::get<0>(collisionList[i]).x > 0) {
	//			e->setVelocityY(0);
	//		}
	//		e->setVelocity(-VECTOR2(e->getVelocity()->x, e->getVelocity()->y) / 2);

	//		


	//	}
	//}


	//if (this->collidesWithTaggedEntities(
	//	"sprite",
	//	"enemy",
	//	collisionList
	//)) {

	//	InkdotEntity* e;

	//	// handle each collision in collision list
	//	for (size_t i = 0; i < collisionList.size(); i++) {

	//		e = entityManager->getEntity(std::get<2>(collisionList[i]));
	//		// bounce off collision vector
	//		e->setVelocity(-VECTOR2(e->getVelocity()->x, e->getVelocity()->y) / 2);
	//	}
	//}

	//if (this->collidesWithTaggedEntities(
	//	"sprite",
	//	"turret",
	//	collisionList
	//)) {

	//	InkdotEntity* e;

	//	// handle each collision in collision list
	//	for (size_t i = 0; i < collisionList.size(); i++) {

	//		e = entityManager->getEntity(std::get<2>(collisionList[i]));
	//		// bounce off collision vector
	//		e->setVelocity(-VECTOR2(e->getVelocity()->x, e->getVelocity()->y)/2);
	//	}
	//}

	//if (this->collidesWithTaggedEntities(
	//	"sprite",
	//	"spikes",
	//	collisionList
	//)) {

	//	InkdotEntity* e;

	//	// handle each collision in collision list
	//	for (size_t i = 0; i < collisionList.size(); i++) {

	//		e = entityManager->getEntity(std::get<2>(collisionList[i]));
	//		// bounce off collision vector
	//		e->setVelocity(-VECTOR2(e->getVelocity()->x, e->getVelocity()->y) / 2);
	//	}
	//
	//}

	// call base method
	InkdotEntity::onUpdateEnd(prevFrameTime);
}