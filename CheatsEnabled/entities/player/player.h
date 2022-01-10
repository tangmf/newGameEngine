#pragma once
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module:			Gameplay Programming
// Assignment1:		Cheats:Enabled
// Student Name:	Josiah Low
// Student No.:		S10204535J
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// ===========================================================================
// InkdotEngine - Player Entity Class Specification
// ===========================================================================

#ifndef _ENTITY_PLAYER_H
#define _ENTITY_PLAYER_H

// include necessary headers
#include "../../inkdot/include.h"
#include "../../inkdot/src/common.h"

// related constructs

namespace PlayerConstants
{
	// texture constants

	const int	WIDTH = 38;
	const int	HEIGHT = 55;
	const int	TEXTURE_COLS = 4;
	const int	START_FRAME = 0;
	const int	END_FRAME = 3;
	const float ANIM_DELAY = 0.2f;

	// initial position

	const int INIT_X = (WindowConstants::GAME_WIDTH - WIDTH) / 2;
	const int INIT_Y = (WindowConstants::GAME_HEIGHT - HEIGHT) / 2;

	// physics constants

	const float SPEED = 10000.0f;
	const float MASS = 3;

	// misc

	const float ROTATION_RATE = (float)GameConstants::PI / 4;
}


// class specification

class Player : public InkdotLegacyEntity
{
private:

	// entity states
	Graphics* graphics;
	SpriteData spriteData;
	COLLISION_LIST collisionList;

public:
	int coincount = 0;
	bool grounded = FALSE;

	// default constructor
	Player();

	// default destructor
	~Player();


	// entity methods

	// initializes the entity - called on scene initialization
	virtual void initialize(Graphics* g, TextureManager* tm);

	// prepares the entity for deletion - called before entity is destroyed
	virtual void cleanup() {}

	// updates the entity - called each frame after scene update() ends
	virtual void onUpdateEnd(float prevFrameTime);

	// handles artificial intelligence for the entity - called each frame
	virtual void ai() {}

	// Return X position.
	virtual float getX() { return spriteData.x; }

	// Return Y position.
	virtual float getY() { return spriteData.y; }

	// Set X location.
	virtual void setX(float newX) { spriteData.x = newX; }

	// Set Y location.
	virtual void setY(float newY) { spriteData.y = newY; }

	// Set rotation angle in degrees.
   // 0 degrees is up. Angles progress clockwise.
	virtual void setDegrees(float deg) { getSprite()->setDegrees(deg); }
	float deg;
	virtual void getDegrees() {return getSprite()->setDegrees(deg); }
	// Set rotation angle in radians.
	// 0 radians is up. Angles progress clockwise.
	virtual void setRadians(float rad) { spriteData.angle = rad; }
};

#endif // !_ENTITY_ENEMY_H