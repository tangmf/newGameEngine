// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module:			Gameplay Programming
// Assignment1:		Cheats:Enabled
// Student Name:	Tang Ming Feng
// Student No.:		S10185023E
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// ===========================================================================
// InkdotEngine - Goals Entity Class Specification
// ===========================================================================

#ifndef _ENTITY_GOAL_H
#define _ENTITY_GOAL_H

// include necessary headers
#include "../../inkdot/include.h"


// related constructs

namespace GoalConstants
{
	// texture constants

	const int WIDTH = 512;
	const int HEIGHT = 512;
	const int TEXTURE_COLS = 1;
	const int START_FRAME = 0;
	const int END_FRAME = 0;
	const int ANIM_DELAY = 0;

	// initial position

	const float INIT_X = (WindowConstants::GAME_WIDTH - WIDTH) / 2.0f;
	const float INIT_Y = (WindowConstants::GAME_HEIGHT - HEIGHT) / 2.0f;

	// physics constants

	const float ROTATION_RATE = (float)GameConstants::PI / 4;
	const float SPEED = 300;
	const float MASS = 0;
}


// class specification

class Goal : public InkdotLegacyEntity
{
private:

	// entity states
	COLLISION_LIST collisionList;
public:

	// default constructor
	Goal();

	// default destructor
	~Goal();


	// entity methods

	// initializes the entity - called on scene initialization
	virtual void initialize(Graphics* g, TextureManager* tm);

	// prepares the entity for deletion - called before entity is destroyed
	virtual void cleanup() {}

	// updates the entity - called each frame before scene update() starts
	virtual void update(float prevFrameTime);

	// handles artificial intelligence for the entity - called each frame
	virtual void ai() {}

};

#endif // !_ENTITY_GOAL_H