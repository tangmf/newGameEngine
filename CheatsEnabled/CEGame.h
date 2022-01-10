// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module:			Gameplay Programming
// Assignment1:		Cheats:Enabled
// Student Name:	Eugene Long - [Engine Designer]
// Student No.:		S10193060J
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// ===========================================================================
// Cheats:Enabled Game Class Specification
// ===========================================================================

#ifndef _CEGAME_H
#define _CEGAME_H

// import necessary headers
#include "inkdot/include.h"
#include "scenes/mainmenu/mainmenu.h"
#include "scenes/test/testscene.h"
#include "scenes/levelcreator/levelcreator.h"
#include "scenes/Load/loadscene.h"
//#include "createlevelscene.h"

// class specification
class CEGame : public Game
{
private:

	// game-wide items go here (anything less should go in a scene)

public:

	// default constructor
	CEGame() {}

	// default destructor
	~CEGame() {}


	// game method extensions

	virtual void initialize(
		HWND	hwnd
	) {
		// call base initialize
		Game::initialize(hwnd);

		// register new scenes here - non-registered scenes will NOT work!
		// e.g:
		// Scene* myScene = new Scene();
		// sceneManager->registerScene(myScene, sceneName);
		//
		// then update the initial game scene in inkdot/common.h
		// (default is that it transitions to have no scene loaded)
		
		TestScene* testScene = new TestScene();
		sceneManager->registerScene(testScene, "TEST_SCENE");

		MainMenuScene* mainMenuScene = new MainMenuScene();
		sceneManager->registerScene(mainMenuScene, SceneNames::MAIN_MENU);

		Scene* levelCreator = new LevelCreatorScene();
		sceneManager->registerScene(levelCreator, "LEVEL_CREATOR");

		Scene* loadScene = new LoadScene();
		sceneManager->registerScene(loadScene, "LOAD_SCENE");
	}
};

#endif // !_CEGAME_H