// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module:			Gameplay Programming
// Assignment1:		Cheats:Enabled
// Student Name:	Tang Ming Feng
// Student No.:		S10185023E
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// ===========================================================================
// InkdotEngine - Bullet Entity Class Implementation
// ===========================================================================

// include specification
#include "bullet.h"

//============================================================================
// default constructor
//============================================================================
Bullet::Bullet():

	// base object constructor
	InkdotLegacyEntity()
{}

//============================================================================
// default destructor
//============================================================================
Bullet::~Bullet() {}


// entity methods

//=============================================================================
// initializes the entity - called on scene initialization
//=============================================================================
void Bullet::initialize(
	Graphics*		g,
	TextureManager*	tm
) {
	// initialize base object
	InkdotLegacyEntity::initialize(
		g,
		BulletConstants::WIDTH,
		BulletConstants::HEIGHT,
		BulletConstants::TEXTURE_COLS,
		tm,
		BulletConstants::INIT_X,
		BulletConstants::INIT_Y,
		BulletConstants::MASS
	);
}

//=============================================================================
// updates the entity - called each frame before scene update() starts
//=============================================================================
void Bullet::update(
	float	prevFrameTime
) {
	// apply entity velocity to its position
	applyVelocity(prevFrameTime);

	// kill bullet on hitting wall
	if (int w = getCollider("sprite")->outsideRect(WindowConstants::BOUNDS))
	{
		setActive(FALSE);
	}

	if (this->collidesWithTaggedEntities(
		"sprite",
		"player",
		collisionList
	)) {

		InkdotEntity* e;

		// handle each collision in collision list
		for (size_t i = 0; i < collisionList.size(); i++) {

			e = entityManager->getEntity(std::get<2>(collisionList[i]));
			// kill bullet
			this->setActive(FALSE);
			// kill player
			e->setActive(FALSE);

			//enemy2.setActive(0);
			//enemy2.setVisible(0);
		}
	}
}
