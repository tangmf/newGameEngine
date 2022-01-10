// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module:			Gameplay Programming
// Assignment1:		Cheats:Enabled
// Student Name:	Tang Ming Feng
// Student No.:		S10185023E
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// ===========================================================================
// InkdotEngine - Enemy Entity Class Specification
// ===========================================================================

#ifndef _ENTITY_ENEMY_H
#define _ENTITY_ENEMY_H

// include necessary headers
#include "../../inkdot/include.h"
#include "../../inkdot/src/common.h"
#include "../../inkdot/src/entity/legacyEntity.h"
#include "../../entities/player/player.h"

// related constructs

namespace EnemyConstants
{
	// texture constants

	const int	WIDTH			= 26;
	const int	HEIGHT			= 26;
	const int	TEXTURE_COLS	= 3;
	const int	START_FRAME		= 0;
	const int	END_FRAME		= 2;
	const float ANIM_DELAY		= 0.2f;

	// initial position

	const int INIT_X	= (WindowConstants::GAME_WIDTH - WIDTH) / 2;
	const int INIT_Y	= (WindowConstants::GAME_HEIGHT - HEIGHT) / 2;

	// physics constants

	const float SPEED	= 100.0f;
	const float MASS	= 3;

	// misc

	const float ROTATION_RATE = (float)GameConstants::PI / 4;
}


// class specification

class Enemy : public InkdotLegacyEntity
{
private:

	// entity states
	Graphics* graphics;
	SpriteData spriteData;
	COLLISION_LIST collisionList;

public:
	Player* player;
	bool playerDetect;

	// default constructor
	Enemy();

	// default destructor
	~Enemy();


	// entity methods

	// initializes the entity - called on scene initialization
	virtual void initialize(Graphics* g, TextureManager* tm, Player* playerptr);

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
	virtual void setDegrees(float deg) { spriteData.angle = deg * ((float)GameConstants::PI / 180.0f); }

	// Set rotation angle in radians.
	// 0 radians is up. Angles progress clockwise.
	virtual void setRadians(float rad) { spriteData.angle = rad; }

	//void targetPlayer(InkdotEntity player, float prevFrameTime) {};
};

#endif // !_ENTITY_ENEMY_H