// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module:			Gameplay Programming
// Assignment1:		Cheats:Enabled
// Student Name:	Tang Ming Feng
// Student No.:		S10185023E
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// ===========================================================================
// InkdotEngine - Goal Entity Class Implementation
// ===========================================================================

// include specification
#include "goal.h"

//============================================================================
// default constructor
//============================================================================
Goal::Goal() :

	// base object constructor
	InkdotLegacyEntity()
{}

//============================================================================
// default destructor
//============================================================================
Goal::~Goal() {}


// entity methods

//=============================================================================
// initializes the entity - called on scene initialization
//=============================================================================
void Goal::initialize(
	Graphics* g,
	TextureManager* tm
) {
	// initialize base object
	InkdotLegacyEntity::initialize(
		g,
		GoalConstants::WIDTH,
		GoalConstants::HEIGHT,
		GoalConstants::TEXTURE_COLS,
		tm,
		GoalConstants::INIT_X,
		GoalConstants::INIT_Y,
		GoalConstants::MASS
	);
}

//=============================================================================
// updates the entity - called each frame before scene update() starts
//=============================================================================
void Goal::update(
	float	prevFrameTime
) {
	// apply entity velocity to its position
	applyVelocity(prevFrameTime);

	if (this->collidesWithTaggedEntities(
		"sprite",
		"player",
		collisionList
	)) {

		InkdotEntity* e;

		// handle each collision in collision list
		for (size_t i = 0; i < collisionList.size(); i++) {

			e = entityManager->getEntity(std::get<2>(collisionList[i]));

			e->setActive(FALSE);


		}
	}

}
