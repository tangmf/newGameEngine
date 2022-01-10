// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module:			Gameplay Programming
// Assignment1:		Cheats:Enabled
// Student Name:	Tang Ming Feng
// Student No.:		S10185023E
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// ===========================================================================
// InkdotEngine - Spikes Entity Class Implementation
// ===========================================================================

// include specification
#include "spikes.h"

//============================================================================
// default constructor
//============================================================================
Spikes::Spikes() :

	// base object constructor
	InkdotLegacyEntity()
{}

//============================================================================
// default destructor
//============================================================================
Spikes::~Spikes() {}


// entity methods

//=============================================================================
// initializes the entity - called on scene initialization
//=============================================================================
void Spikes::initialize(
	Graphics* g,
	TextureManager* tm
) {
	// initialize base object
	InkdotLegacyEntity::initialize(
		g,
		SpikesConstants::WIDTH,
		SpikesConstants::HEIGHT,
		SpikesConstants::TEXTURE_COLS,
		tm,
		SpikesConstants::INIT_X,
		SpikesConstants::INIT_Y,
		SpikesConstants::MASS
	);
	addTag("spikes");
}

//=============================================================================
// updates the entity - called each frame before scene update() starts
//=============================================================================
void Spikes::update(
	float	prevFrameTime
) {
	// apply entity velocity to its position
	applyVelocity(prevFrameTime);

	// bounce off walls
	if (int w = getCollider("sprite")->outsideRect(WindowConstants::BOUNDS))
	{
		switch (w)
		{
		case 1: if (velocity.x < 0) setVelocityX(-velocity.x); break;
		case 2: if (velocity.y < 0) setVelocityY(-velocity.y); break;
		case 3: if (velocity.x > 0) setVelocityX(-velocity.x); break;
		case 4: if (velocity.y > 0) setVelocityY(-velocity.y); break;
		}
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
			// kill player
			e->setActive(FALSE);
		}
	}
}
