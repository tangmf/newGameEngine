// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module:			Gameplay Programming
// Assignment1:		Cheats:Enabled
// Student Name:	Tang Ming Feng
// Student No.:		S10185023E
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// ===========================================================================
// InkdotEngine - Turret Entity Class Specification
// ===========================================================================

#ifndef _ENTITY_TURRET_H
#define _ENTITY_TURRET_H

// include necessary headers
#include "../../inkdot/include.h"
#include "../bullet/bullet.h"
#include <vector>


// related constructs

namespace TurretConstants
{
	// texture constants

	const int WIDTH			= 480;
	const int HEIGHT		= 288;
	const int TEXTURE_COLS	= 1;
	const int START_FRAME	= 0;
	const int END_FRAME		= 0;
	const int ANIM_DELAY	= 0;

	// initial position

	const int INIT_X	= (WindowConstants::GAME_WIDTH - WIDTH) / 2;
	const int INIT_Y	= (WindowConstants::GAME_HEIGHT - HEIGHT) / 2;

	// physics constants

	const float ROTATION_RATE	= (float)GameConstants::PI / 4;
	const float SPEED			= 100;
	const float MASS			= 3;
}

// inherits from Entity class
class Turret : public InkdotLegacyEntity
{
private:

	// entity states

	float shotTimer;
	float cooldownTime;
	COLLISION_LIST collisionList;

	// bullet states

	TextureManager bulletTexture;
	std::vector<Bullet*> bulletList;

public:

	// default constructor
	Turret();

	// default destructor
	~Turret();


	// game logic methods

	// shoots a bullet
	void shoot(Bullet* bullet, float x, float y, bool flip);


	// entity methods

	// initializes the entity - called on scene initialization
	virtual void initialize(Graphics* g, TextureManager* tm);

	// prepares the entity for deletion - called before entity is destroyed
	virtual void cleanup() {}

	// updates the entity - called each frame after scene update() ends
	virtual void onUpdateEnd(float prevFrameTime);

	// handles artificial intelligence for the entity - called each frame
	virtual void ai() {}

	// releases all memory reserved for graphics objects so that the bound
	// graphics device can be reset - called when the graphics device is lost
	virtual void releaseAllGraphics();

	// recreates and restores all graphics objects - called when the lost
	// graphics device is restored
	virtual void resetAllGraphics();

	// getters

	int getBulletCount()
	{ return bulletList.size(); }
};

#endif // !_ENTITY_TURRET_H