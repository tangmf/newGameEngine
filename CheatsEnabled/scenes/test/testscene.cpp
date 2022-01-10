// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module:			Gameplay Programming
// Assignment1:		Cheats:Enabled
// Student Name:	Ming Feng, Josiah Low
// Student No.:		S10185023E, S10204535J
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// ===========================================================================
// InkdotEngine - Test Scene Specification
// ===========================================================================

// include specification
#include "testscene.h"

// ===========================================================================
// default constructor
// ===========================================================================
TestScene::TestScene():

	// text states
	HUDText			(),
	options			(),
	optionSpacing	(20),

	// texture states
	enemyTexture	(),
	coinTexture		(),
	playerTexture	(),
	hud1Texture		(),
	hud2Texture		(),
	turretTexture	(),
	ability1Texture	(),
	ability2Texture	(),


	// entities
	enemy1			(),
	player			(),
	coin1			(),
	hud1			(),
	hud2			(),
	turret1			(),
	ability1		(),
	ability2		(),


	// game logic
	coinCount		(0),
	playerVelocity	(100),

	// timer
	seconds			(0),
	minutes			(0),

	// working states
	collisionList	()
{}

// ===========================================================================
// default destructor
// ===========================================================================
TestScene::~TestScene() {}


// scene methods

// ===========================================================================
// set up scene objects during scene initialization
// ===========================================================================
void TestScene::setup()
{
	// configure scene
	getGraphics()->setBackgroundColor(CommonColors::YELLOW);

	// initialize text fonts

	if (!HUDText.initialize(
		getGraphics(),
		13,
		true,
		false,
		L"Terminal"
	)) throw (
		InkdotError(
			InkdotErrorType::FATAL_ERROR,
			"Error initializing hud font!"
		)
		);

	if (!pauseText.initialize(
		getGraphics(),
		13,
		true,
		false,
		L"Terminal"
	)) throw (
		InkdotError(
			InkdotErrorType::FATAL_ERROR,
			"Error initializing pause font!"
		)
		);
	pauseText;

	// register menu options

	options.push_back(MenuOption(L"resume", &pauseText));
	options.push_back(MenuOption(L"quit", &pauseText));


	options[selectedMenuIndex].selected = true;

	// calculate positions for each option

	int optionHeight = pauseText.print(L"A", NULL, DT_CALCRECT);
	int totalMenuHeight =
		options.size() * (optionHeight + optionSpacing) - optionSpacing;
	int startYPos = (WindowConstants::GAME_HEIGHT - totalMenuHeight) / 2;

	for (size_t i = 0; i < options.size(); i++) {
		options[i].xPos = (WindowConstants::GAME_WIDTH - options[i].labelWidth) / 2;
		options[i].yPos = (startYPos + (i * (optionHeight + optionSpacing)));
	}


	// initialize textures

	// enemy
	if (!enemyTexture.initialize(
		getGraphics(), 
		L"./assets/enemy.jpg"
	)) 
		throw(
		InkdotError(
			InkdotErrorType::FATAL_ERROR, 
			"Error initializing enemy texture!"
		));

	// coin
	if (!coinTexture.initialize(
		getGraphics(),
		L"./assets/coin.png"
	))
		throw(
			InkdotError(
				InkdotErrorType::FATAL_ERROR,
				"Error initializing coin texture!"
			));

	// player
	// TODO: REPLACE THIS!
	if (!playerTexture.initialize(
		getGraphics(),
		L"./assets/player.jpg"
	))
		throw(
			InkdotError(
				InkdotErrorType::FATAL_ERROR,
				"Error initializing player texture!"
			));

	// hud's
	if (!hud1Texture.initialize(
		getGraphics(),
		L"./assets/HUD.png"
	))
		throw(
			InkdotError(
				InkdotErrorType::FATAL_ERROR,
				"Error initializing HUD texture!"
			));

	if (!hud2Texture.initialize(
		getGraphics(),
		L"./assets/HUD.png"
	))
		throw(
			InkdotError(
				InkdotErrorType::FATAL_ERROR,
				"Error initializing HUD texture!"
			));

	// turret
	if (!turretTexture.initialize(
		getGraphics(),
		L"./assets/turret.png"
	))
		throw(
			InkdotError(
				InkdotErrorType::FATAL_ERROR,
				"Error initializing turret texture!"
			));

	// abilities
	if (!ability1Texture.initialize(
		getGraphics(),
		L"./assets/gravityarrow.png"
	))
		throw(
			InkdotError(
				InkdotErrorType::FATAL_ERROR,
				"Error initializing gravity arrow texture!"
			));

	if (!ability2Texture.initialize(
		getGraphics(),
		L"./assets/yellowarrow.png"
	))
		throw(
			InkdotError(
				InkdotErrorType::FATAL_ERROR,
				"Error initializing yellow arrow texture!"
			));

	// initialize objects

	coin1.initialize(getGraphics(), &coinTexture);
	coin1.setScale(0.1f);
	coin1.setCenter(VECTOR2(
		WindowConstants::GAME_WIDTH / 2.0f,
		WindowConstants::GAME_HEIGHT / 1.5f
	));

	turret1.initialize(getGraphics(), &turretTexture);
	turret1.setScale(0.1f);
	turret1.setCenter(VECTOR2(
		WindowConstants::GAME_WIDTH / 1.5f,
		WindowConstants::GAME_HEIGHT / 1.5f
	));
	turret1.getSprite()->flipHorizontal(TRUE);

	

	player.initialize(getGraphics(), &playerTexture);
	player.setCenter(VECTOR2(
		WindowConstants::GAME_WIDTH / 6.0f,
		WindowConstants::GAME_HEIGHT / 2.0f
	));

	hud1.initialize(getGraphics(), &hud1Texture);
	hud1.setScale(0.2f);
	hud1.setCenter(VECTOR2(
		WindowConstants::GAME_WIDTH / 2,
		430.0f
	));

	hud2.initialize(getGraphics(), &hud2Texture);
	hud2.setScale(0.2f);
	hud2.setCenter(VECTOR2(
		(WindowConstants::GAME_WIDTH / 2) + 57,
		430.0f
	));

	ability1.initialize(getGraphics(), &ability1Texture);
	ability1.setScale(0.05f);
	ability1.setCenter(VECTOR2(
		WindowConstants::GAME_WIDTH / 2,
		430.0f
	));

	ability2.initialize(getGraphics(), &ability2Texture);
	ability2.setScale(0.05f);
	ability2.setCenter(VECTOR2(
		(WindowConstants::GAME_WIDTH / 2)+57,
		430.0f
	));

	// add entities to entity manager
	entityManager.addEntity(&coin1);
	entityManager.addEntity(&turret1);
	//entityManager.addEntity(&enemy1);
	entityManager.addEntity(&player);
	entityManager.addEntity(&hud1);
	entityManager.addEntity(&hud2);
	entityManager.addEntity(&ability1);
	entityManager.addEntity(&ability2);

	//initialize escKey
	escKey = false;

	//initialize cleanup bool
	clean = false;
	

	//initialize gravity bool
	grav = false;
}

// ===========================================================================
// clean up scene objects and prepare to transit out of scene
// ===========================================================================
void TestScene::cleanup()
{
	// reset bg color
	getGraphics()->setBackgroundColor(WindowConstants::BACK_COLOR);

	if (clean == true) {
		getEntityManager()->deleteAllEntities();
	}
}

// ===========================================================================
// update objects for each frame
// ===========================================================================
void TestScene::update(
	float	prevFrameTime
) {
	// resistance

	/*if (*enemy1.getVelocity() > VECTOR2(0, 0)) {
		enemy1.setVelocity(*enemy1.getVelocity() * 0.9f);
	}
	if (*player.getVelocity() > VECTOR2(0, 0)) {
		player.setVelocity(*player.getVelocity() * 0.9f);
	}
	if (*turret1.getVelocity() > VECTOR2(0, 0)) {
		turret1.setVelocity(*turret1.getVelocity() * 0.9f);
	}*/

	// gravity


	player.applySceneGravity(prevFrameTime);
	turret1.applySceneGravity(prevFrameTime);

	// player control

	if (getInput()->isKeyDown(0x41))
	{
		/*enemy2.flipHorizontal(1);
		enemy2.setDeltaV(VECTOR2(-enemyNS::SPEED, 0));*/

		player.setVelocityX(-playerVelocity);
	}

	else if (getInput()->isKeyDown(0x44))
	{
		/*enemy2.flipHorizontal(0);
		enemy2.setDeltaV(VECTOR2(enemyNS::SPEED, 0));*/

		player.setVelocityX(playerVelocity);
	}

	else if (getInput()->isKeyDown(0x57))
	{
		/*enemy2.setVelocity(enemy2.getVelocity() 
		+ VECTOR2(0, -enemyNS::SPEED * 4));*/

		player.setVelocityY(-playerVelocity);
	}

	// call the pause menu
	if (getInput()->isKeyDown(0x1B))	//escape key
	{
		escKey = true;
	}

	//player abilities

	//teleport ability
	if (getInput()->isKeyDown(0x45)) // E key, used for teleporting
	{
		//player.setX(getInput()->getMouseX());
		//player.setY(getInput()->getMouseY());
		VECTOR2 teleport = -VECTOR2(player.getCenter()->x, player.getCenter()->y) + VECTOR2(getInput()->getMouseX(), getInput()->getMouseY());
		if (teleport >= VECTOR2(10, 10)) {
			player.setCenter(VECTOR2(10, 10));
		}
		else {
			player.setCenter(VECTOR2(getInput()->getMouseX(), getInput()->getMouseY()));
		}
		
	}

	//invert gravity ability
	if (getInput()->wasKeyPressed(0x46) && grav == false) // F key, used for flipping the gravity
	{
		player.setRotation(GameConstants::PI);
		player.setGravity(player.getGravity() * -1);
		grav = true;
		time = 0;
	}

	time += prevFrameTime;
	/*time+= prevFrameTime*/;
	if (time > 10 && grav == true) {
		player.setRotation(0);
		player.setGravity(player.getGravity() * -1);
		grav = false;
	}

	// timer 
	if (escKey == true)
	{
		seconds += 0;
	}
	else 
	{
		seconds += prevFrameTime;
	}
	if (seconds >= 60) {

		// increment minutes
		minutes++;
		seconds = 0;
	}

	// handle user input

	if (getInput()->wasKeyPressed(VK_UP)) {
		updateSelectedStates(-1);
	}
	else if (getInput()->wasKeyPressed(VK_DOWN)) {
		updateSelectedStates(1);
	}
	else if (getInput()->wasKeyPressed(VK_RETURN)) {

		// handle selection
		switch (selectedMenuIndex) {

			// 0: resume game
		case 0: entityManager.setPaused(false); escKey = false; break;
			// 1: return to main menu
		case 1: sceneManager->transitionToScene(SceneNames::MAIN_MENU); clean = true; break;
		}

	}
}


void TestScene::updateSelectedStates(
	int indexModifier
) {
	int pIndex = selectedMenuIndex;
	selectedMenuIndex += indexModifier;

	// ensure index is not negative
	if (selectedMenuIndex < 0) selectedMenuIndex = options.size() - 1;

	// ensure index does not exceed range of options
	// negative sign is used to flip sign bit for unsigned comparison
	else if ((size_t)-selectedMenuIndex > options.size())
		selectedMenuIndex %= options.size();

	// if selection did not change, return early.
	if (selectedMenuIndex == pIndex) return;

	// update option states accordingly
	options[pIndex].selected = false;
	options[selectedMenuIndex].selected = true;
}

// ===========================================================================
// handle collisions for objects on each frame
// ===========================================================================
void TestScene::collisions()
{
	VECTOR2 collisionVector;

	if (player.getCollider("sprite")->collidesWith(
		*coin1.getCollider("sprite"),
		collisionVector
	)) {
		coinCount += 1;
		coin1.setActive(0);
	}

	/*if (player.getCollider("sprite")->collidesWith(
		*enemy1.getCollider("sprite"),
		collisionVector
	)) {
		player.bounce(collisionVector, enemy1);
	}*/

	if (player.getCollider("sprite")->collidesWith(
		*turret1.getCollider("sprite"),
		collisionVector
	)) {
		player.bounce(collisionVector, enemy1);
	}

	if (player.collidesWithTaggedEntities(
		"sprite",
		"bullet",
		collisionList
	)) {

		InkdotEntity* e;

		// handle each collision in collision list
		for (size_t i = 0; i < collisionList.size(); i++) {

			e = entityManager.getEntity(std::get<2>(collisionList[i]));

			// bounce off collision vector
			player.bounce(
				std::get<0>(collisionList[i]),
				*e
			);

			// kill bullet
			e->setActive(FALSE);

			//enemy2.setActive(0);
			//enemy2.setVisible(0);
		}
	}
}

// ===========================================================================
// render sprites for scene foreground: draws over entity sprites
// ===========================================================================
void TestScene::renderForeground()
{
	// draw HUD
	HUDText.print(
		L"Time: " + 
		std::to_wstring(int(minutes)) + 
		L":" + 
		std::to_wstring(int(seconds)),
		10, 30);
	HUDText.print(L"Coins: " + std::to_wstring(coinCount), 10, 50);
	HUDText.print(
		L"Bullets: " + 
		std::to_wstring(turret1.getBulletCount()), 
		10, 100);
	
	//keybinds for player abilities
	HUDText.print(L"F", WindowConstants::GAME_WIDTH / 2, 390);
	HUDText.print(L"E", (WindowConstants::GAME_WIDTH / 2) + 57, 390);

	if (escKey == true) {
		entityManager.setPaused(true);
		/*Sleep(10000000);*/
		getGraphics()->drawPrimitiveRect(
			WindowConstants::GAME_WIDTH / 2- options[0].labelWidth,
			WindowConstants::GAME_HEIGHT / 2 - 40,
			GameConstants::MAX_Z_DEPTH,
			2*(options[0].labelWidth),
			80,
			CommonColors::GRAY);

		// render each option in the menu
		for (size_t i = 0; i < options.size(); i++) {
			(WindowConstants::GAME_WIDTH - options[i].labelWidth) / 2;
			options[i].print();
		}
	}


}

// ===========================================================================
// releases all memory reserved for graphics objects so that the bound
// graphics device can be reset. called when the graphics device is lost.
// ===========================================================================
void TestScene::releaseAllGraphics()
{
	turretTexture.onLostDevice();
	coinTexture.onLostDevice();
	playerTexture.onLostDevice();
	hud1Texture.onLostDevice();
	hud2Texture.onLostDevice();
	turretTexture.onLostDevice();
}

// ===========================================================================
// recreates and restores all graphics objects. called after a lost
// graphics device is restored.
// ===========================================================================
void TestScene::resetAllGraphics()
{
	turretTexture.onResetDevice();
	coinTexture.onResetDevice();
	playerTexture.onResetDevice();
	hud1Texture.onResetDevice();
	hud2Texture.onResetDevice();
	turretTexture.onResetDevice();
}
