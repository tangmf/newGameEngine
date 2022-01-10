// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module:			Gameplay Programming
// Assignment1:		Cheats:Enabled
// Student Name:	Eugene Long - [Engine Designer]
// Student No.:		S10193060J
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// ===========================================================================
// InkdotEngine - Scene Abstract Class Specification
// ===========================================================================
// * This is a InkdotEngine component, derived from the standard GPP engine
//   templates provided by Ms. Lee Ching Yun, modified by Eugene Long.
// ===========================================================================

#ifndef _INKDOT_SCENE_H
#define _INKDOT_SCENE_H

// import minimal windows headers
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// import necessary headers
#include "../graphics/graphics.h"
#include "../graphics/text.h"
#include "../input/input.h"
#include "../common.h"
#include "../error.h"
#include "../interfaces/ISceneManager.h"
#include "../interfaces/IScene.h"
#include "../entity/entityManager.h"
#include <string>


// class specification

class Scene : public IScene
{
protected:

	// handlers

	// pointer to the associated sceneManager handler
	ISceneManager* sceneManager;

	// entity manager for the current scene
	EntityManager entityManager;

public:

	// default constructor
	Scene();

	// default destructor
	~Scene();


	// extensible scene methods

	/// <summary>
	/// Initializes the Scene object instance
	/// </summary>
	virtual void initialize();

	// deallocate all manually allocated scene objects and reset object to the
	// pre-initialized state. will be called on instance destruction.
	virtual void deleteAll();


	// abstract scene methods

	// set up scene objects during scene initialization
	virtual void setup() = 0;

	// clean up scene objects and prepare to transit out of scene
	virtual void cleanup() = 0;

	// update objects for each frame
	virtual void update(float prevFrameTime) = 0;

	// handle artificial intelligence on each frame
	virtual void ai() = 0;

	// handle collisions for objects on each frame
	virtual void collisions() = 0;

	// render sprites for scene foreground: draws over entity sprites
	virtual void renderForeground() = 0;

	// render sprites for scene background: draws under entity sprites
	virtual void renderBackground() = 0;

	// releases all memory reserved for graphics objects so that the bound
	// graphics device can be reset. called when the graphics device is lost.
	virtual void releaseAllGraphics() = 0;

	// recreates and restores all graphics objects. called after a lost
	// graphics device is restored.
	virtual void resetAllGraphics() = 0;


	// IScene getters

	// returns the graphics handler for the scene
	Graphics* getGraphics() const
	{
		// ensure sceneManager is bound
		if (!sceneManager) throw(
			InkdotError(
				InkdotErrorType::FATAL_ERROR,
				"Error: SceneManager not bound for this Scene!"
			)
		);

		return sceneManager->getGraphics(); 
	}

	// returns the input handler for the scene
	Input* getInput() const
	{
		// ensure sceneManager is bound
		if (!sceneManager) throw(
			InkdotError(
				InkdotErrorType::FATAL_ERROR,
				"Error: SceneManager not bound for this Scene!"
			)
		);

		return sceneManager->getInput();
	}

	// returns the entity manager for the scene
	EntityManager* getEntityManager()
	{ return &entityManager; }


	// setters

	// sets the sceneManager for the scene
	void setSceneManager(ISceneManager* sceneMgr)
	{ sceneManager = sceneMgr; }

};

#endif // !_INKDOT_SCENE_H