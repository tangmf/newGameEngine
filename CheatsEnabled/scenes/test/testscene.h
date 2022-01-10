// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module:			Gameplay Programming
// Assignment1:		Cheats:Enabled
// Student Name:	Ming Feng, Josiah Low
// Student No.:		S10185023E, S10204535J
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// ===========================================================================
// InkdotEngine - Test Scene Specification
// ===========================================================================

#ifndef _SCENE_TEST_H
#define _SCENE_TEST_H

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
#include "../mainmenu/mainmenu.h"
#include "../../inkdot/src/graphics/graphics.h"
#include <string>

// class specification

class TestScene : public Scene
{
private:

	// text states

	Text HUDText;
	Text pauseText;

	// texture states

	TextureManager enemyTexture;
	TextureManager coinTexture;
	TextureManager playerTexture;
	TextureManager hud1Texture;
	TextureManager hud2Texture;
	TextureManager turretTexture;
	TextureManager ability1Texture;
	TextureManager ability2Texture;

	// entities

	Enemy	enemy1;
	Player	player;
	Coin	coin1;
	Hud	hud1;
	Hud	hud2;
	Turret	turret1;
	Abilities ability1;
	Abilities ability2;


	// game logic states
	
	int		coinCount;
	float	playerVelocity;

	// timer states

	float	seconds;
	int		minutes;
	float	time;
	// working states

	COLLISION_LIST collisionList;


public:

	// default constructor
	TestScene();

	// default destructor
	~TestScene();

	
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

	// menu options
	std::vector<MenuOption> options;

	// option spacing
	int optionSpacing;

	// option selected prefix
	std::wstring prefixStr;

	// selected menu option index
	int selectedMenuIndex;

	// update menu selection states
	void updateSelectedStates(int indexModifier);

	bool escKey = false;
	bool clean = false;
	bool grav = false;
};

#endif // !_SCENE_TEST_H