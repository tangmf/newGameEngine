// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module:			Gameplay Programming
// Assignment1:		Cheats:Enabled
// Student Name:	Tang Ming Feng
// Student No.:		S10185023E
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// ===========================================================================
// InkdotEngine - Coin Entity Class Specification
// ===========================================================================

#ifndef _ENTITY_COIN_H
#define _ENTITY_COIN_H

// include necessary headers
#include "../../inkdot/include.h"


// related constructs

namespace CoinConstants
{
	// texture constants

	const int WIDTH			= 240;
	const int HEIGHT		= 240;
	const int TEXTURE_COLS	= 1;
	const int START_FRAME	= 0;
	const int END_FRAME		= 0;
	const int ANIM_DELAY	= 0;

	// initial position

	const int INIT_X	= (WindowConstants::GAME_WIDTH - WIDTH) / 2;
	const int INIT_Y	= (WindowConstants::GAME_HEIGHT - HEIGHT) / 2;

	// physics constants

	const float MASS	= 1;

	// misc

	const float COLLIDER_RADIUS = 32 / 2;
}


// class specification

class Coin : public InkdotLegacyEntity
{
private:

	// entity states

public:

	// default constructor
	Coin();

	// default destructor
	~Coin();


	// entity methods

	// initializes the entity - called on scene initialization
	virtual void initialize(Graphics* g, TextureManager* tm);

	// prepares the entity for deletion - called before entity is destroyed
	virtual void cleanup() {}

	// updates the entity - called each frame before scene update() starts
	virtual void update(float prevFrameTime) {}

	// handles artificial intelligence for the entity - called each frame
	virtual void ai() {}
};

#endif // !_ENTITY_COIN_H