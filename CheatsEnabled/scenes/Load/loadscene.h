// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module:			Gameplay Programming
// Assignment1:		Cheats:Enabled
// Student Name:	Tang Ming Feng
// Student No.:		S10185023E
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// ===========================================================================
// InkdotEngine - Load Scene Specification
// ===========================================================================

#ifndef _SCENE_LOAD_H
#define _SCENE_LOAD_H

// import necessary headers
#include "../../inkdot/include.h"
#include "../../entities/bullet/bullet.h"
#include "../../entities/coin/coin.h"
#include "../../entities/enemy/enemy.h"
#include "../../entities/turret/turret.h"
#include "../../entities/abilities/abilities.h"
#include "../../entities/hud/hud.h"
#include "../../entities/player/player.h"
#include "../../entities/wall/wall.h"
#include "../../entities/spikes/spikes.h"
#include "../../entities/goal/goal.h"
#include "../../inkdot/src/graphics/graphics.h"
#include "../../utils/levelConverter.h"

#include <string>

// class specification

class LoadScene : public Scene
{
private:

	// text states

	Text HUDText;
	Text pauseText;

	// texture states

	TextureManager hud1Texture;
	TextureManager hud2Texture;


	Hud	hud1;
	Hud	hud2;

	// game logic states

	int		coinCount;
	float	playerVelocity;

	// timer states

	float	seconds;
	int		minutes;

	// working states

	COLLISION_LIST collisionList;

	Player* playerptr;

	// win
	bool win;


public:

	// default constructor
	LoadScene();

	// default destructor
	~LoadScene();


	// methods

	// scene methods

	// deallocate all manually allocated scene objects and reset object to the
	// pre-initialized state. will be called on instance destruction.
	virtual void deleteAll() {};


	// abstract scene methods

	// set up scene objects during scene initialization
	virtual void setup();

	// clean up scene objects and prepare to transit out of scene
	virtual void cleanup();

	// update objects for each frame
	virtual void update(float prevFrameTime);

	// handle artificial intelligence on each frame
	virtual void ai() {}

	// handle collisions for objects on each frame
	virtual void collisions();

	// render sprites for scene foreground: draws over entity sprites
	virtual void renderForeground();

	// render sprites for scene background: draws under entity sprites
	virtual void renderBackground() {}

	// releases all memory reserved for graphics objects so that the bound
	// graphics device can be reset. called when the graphics device is lost.
	virtual void releaseAllGraphics();

	// recreates and restores all graphics objects. called after a lost
	// graphics device is restored.
	virtual void resetAllGraphics();

	float escKey = false;

	// convert level to string
	static std::string convertLevelToString();

	// convert level to string
	static void convertStringToLevel(EntityManager& entityManager, std::string s, Player* player);

	static std::vector<std::string> split_string_by(const std::string& str, char delim);
};

#endif // !_SCENE_LOAD_H