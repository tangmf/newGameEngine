// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module:			Gameplay Programming
// Assignment1:		Cheats:Enabled
// Student Name:	Josiah Low
// Student No.:		S10204535J
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// ===========================================================================
// InkdotEngine - Abilities Entity Class Specification
// ===========================================================================

#ifndef _ENTITY_HUD_H
#define _ENTITY_HUD_H

// include necessary headers
#include "../../inkdot/include.h"



// related constructs

namespace HudConstants
{
	// texture constants

	const int WIDTH = 840;
	const int HEIGHT = 859;
	const int TEXTURE_COLS = 1;
	const int START_FRAME = 0;
	const int END_FRAME = 0;
	const int ANIM_DELAY = 0;

	// initial position

	const int INIT_X = (WindowConstants::GAME_WIDTH - WIDTH) / 2;
	const int INIT_Y = (WindowConstants::GAME_HEIGHT - HEIGHT) / 2;

	// physics constants

	const float ROTATION_RATE = (float)GameConstants::PI / 4;
	/*const float SPEED = 100;
	const float MASS = 3;*/
}

// inherits from Entity class
class Hud : public InkdotLegacyEntity
{
private:

	// entity states

public:

	// default constructor
	Hud();

	// default destructor
	~Hud();


	// game logic methods

	// entity methods

	// initializes the entity - called on scene initialization
	virtual void initialize(Graphics* g, TextureManager* tm);

	// prepares the entity for deletion - called before entity is destroyed
	virtual void cleanup() {}

	// updates the entity - called each frame after scene update() ends
	virtual void update(float prevFrameTime);

	// handles artificial intelligence for the entity - called each frame
	virtual void ai() {}

	// getters

};

#endif // !_ENTITY_HUD_H