// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module:			Gameplay Programming
// Assignment1:		Cheats:Enabled
// Student Name:	Eugene Long - [Engine Designer]
// Student No.:		S10193060J
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// ===========================================================================
// InkdotEngine - Scene Abstract Class Implementation
// ===========================================================================
// * This is a InkdotEngine component, derived from the standard GPP engine
//   templates provided by Ms. Lee Ching Yun, modified by Eugene Long.
// ===========================================================================

// include specification
#include "scene.h"

// ===========================================================================
// default constructor
// ===========================================================================
Scene::Scene():

	// handlers
	sceneManager	(NULL),
	entityManager	()
{}

// ===========================================================================
// default destructor
// ===========================================================================
Scene::~Scene()
{
	// deallocate all manually allocated scene objects
	deleteAll();
}


// extensible scene methods

// ===========================================================================
// initializes the Scene object instance
// ===========================================================================
void Scene::initialize()
{
	// bind entity manager
	entityManager.initialize(this);

	// setup scene objects
	setup();
}

// ===========================================================================
// deallocate all manually allocated scene objects and reset object to the
// pre-initialized state. will be called on instance destruction.
// ===========================================================================
void Scene::deleteAll()
{
	// release memory for all graphics objects and textures
	releaseAllGraphics();
}
