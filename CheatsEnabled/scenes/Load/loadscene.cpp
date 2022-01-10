// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module:			Gameplay Programming
// Assignment1:		Cheats:Enabled
// Student Name:	Tang Ming Feng
// Student No.:		S10185023E
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// ===========================================================================
// InkdotEngine -  Load Scene Class Implementation
// ===========================================================================

// include specification
#include "loadscene.h"

// ===========================================================================
// default constructor
// ===========================================================================
LoadScene::LoadScene() :

	// text states
	HUDText(),

	// texture states

	hud1Texture(),
	hud2Texture(),
	win(),



	// game logic
	coinCount(0),
	playerVelocity(100),

	// timer
	seconds(0),
	minutes(0),

	// working states
	collisionList(),

	// player pointer
	playerptr()
{}

// ===========================================================================
// default destructor
// ===========================================================================
LoadScene::~LoadScene() {}


// scene methods

// ===========================================================================
// set up scene objects during scene initialization
// ===========================================================================
void LoadScene::setup()
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

	// initialize textures


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

	// initialize objects

	
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

	Player* player = new Player();
	TextureManager* playerTexture = new TextureManager();
	playerTexture->initialize(entityManager.getGraphics(),
		L"./assets/player.jpg");
	player->initialize(entityManager.getGraphics(), playerTexture);
	/*player->setScale(0.5);*/
	player->setCenter(VECTOR2(
		PlayerConstants::INIT_X,
		PlayerConstants::INIT_Y)
	);

	entityManager.addEntity(player, 0);

	convertStringToLevel(entityManager, "WALL,500,150,320,10\nWALL,0,330,320,10\nSPAWN_PLAYER,500,100,10,10\nTURRET_SPAWNL,550,400,5,5\nCOIN,200,200,5,5\nSPAWN_HUNTER,400,300,5,5\nSPIKES,300,300,5,5\nGOAL,50,450,10,10", player);
	playerptr = player;
	//initialize escKey
	escKey = false;
}

// ===========================================================================
// clean up scene objects and prepare to transit out of scene
// ===========================================================================
void LoadScene::cleanup()
{
	// reset bg color
	getGraphics()->setBackgroundColor(WindowConstants::BACK_COLOR);
}

// ===========================================================================
// update objects for each frame
// ===========================================================================
void LoadScene::update(
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

	// player control

	
	// timer
	seconds += prevFrameTime;
	if (seconds >= 60) {

		// increment minutes
		minutes++;
		seconds = 0;
	}

	entityManager.updateAllEntities(prevFrameTime);
}

// ===========================================================================
// handle collisions for objects on each frame
// ===========================================================================
void LoadScene::collisions()
{
	VECTOR2 collisionVector;

	//if (player.getCollider("sprite")->collidesWith(
	//	*coin1.getCollider("sprite"),
	//	collisionVector
	//)) {
	//	coinCount += 1;
	//	coin1.setActive(0);
	//}

	//if (player.getCollider("sprite")->collidesWith(
	//	*enemy1.getCollider("sprite"),
	//	collisionVector
	//)) {
	//	player.bounce(collisionVector, enemy1);
	//}

	//if (player.getCollider("sprite")->collidesWith(
	//	*turret1.getCollider("sprite"),
	//	collisionVector
	//)) {
	//	player.bounce(collisionVector, enemy1);
	//}

	//if (player.collidesWithTaggedEntities(
	//	"sprite",
	//	"bullet",
	//	collisionList
	//)) {

	//	InkdotEntity* e;

		// handle each collision in collision list
		//for (size_t i = 0; i < collisionList.size(); i++) {

		//	e = entityManager.getEntity(std::get<2>(collisionList[i]));

		//	// bounce off collision vector
		//	player.bounce(
		//		std::get<0>(collisionList[i]),
		//		*e
		//	);

		//	// kill bullet
		//	e->setActive(FALSE);

		//	//enemy2.setActive(0);
		//	//enemy2.setVisible(0);
		//}
	//}
}

// ===========================================================================
// render sprites for scene foreground: draws over entity sprites
// ===========================================================================
void LoadScene::renderForeground()
{
	// draw HUD
	HUDText.print(
		L"Time: " +
		std::to_wstring(int(minutes)) +
		L":" +
		std::to_wstring(int(seconds)),
		10, 30);
	HUDText.print(L"Coins: " + std::to_wstring(playerptr->coincount), 10, 50);
	if (escKey == true) {
		Sleep(10000000);
		getGraphics()->drawPrimitiveRect(
			WindowConstants::GAME_WIDTH / 2,
			WindowConstants::GAME_HEIGHT / 2,
			GameConstants::MAX_Z_DEPTH,
			70,
			52,
			CommonColors::GRAY);
		pauseText.print(L"resume", WindowConstants::GAME_WIDTH / 2, WindowConstants::GAME_HEIGHT / 2);
		pauseText.print(L"exit", (WindowConstants::GAME_WIDTH / 2), (WindowConstants::GAME_HEIGHT / 2) + 40);

		//pauseText;

		//// register menu options

		//options.push_back(MenuOption(L"begin", &pauseText));
		//options.push_back(MenuOption(L"level creator", &pauseText));
		//options.push_back(MenuOption(L"quit", &pauseText));


		//options[selectedMenuIndex].selected = true;
	}


}

// ===========================================================================
// releases all memory reserved for graphics objects so that the bound
// graphics device can be reset. called when the graphics device is lost.
// ===========================================================================
void LoadScene::releaseAllGraphics()
{

}

// ===========================================================================
// recreates and restores all graphics objects. called after a lost
// graphics device is restored.
// ===========================================================================
void LoadScene::resetAllGraphics()
{

}


void LoadScene::convertStringToLevel(EntityManager& entityManager, std::string s, Player* player) {

	std::vector<std::string> rows = LoadScene::split_string_by(s, '\n');
	for (int i = 0; i < rows.size(); i++) {
		std::vector<std::string> cols = LoadScene::split_string_by(rows[i], ',');
		if (cols[0] == "WALL") {
			Wall* wall = new Wall();
			wall->initialize(std::stof(cols[1]), std::stof(cols[2]), std::stof(cols[3]), std::stof(cols[4]));
			entityManager.getGraphics()->drawPrimitiveRect(
				std::stof(cols[1]) - std::stof(cols[3]),
				std::stof(cols[2]) - std::stof(cols[4]),
				std::stof(cols[3]) * 2,
				std::stof(cols[4]) * 2,
				CommonColors::WHITE,
				CommonColors::WHITE,
				0);
			wall->setCenter(VECTOR2(std::stof(cols[1]), std::stof(cols[2])));
			entityManager.addEntity(wall, 0);
		}
		else if (cols[0] == "SPAWN_PLAYER") {
			player->setCenter(VECTOR2(
				std::stof(cols[1]),
				std::stof(cols[2])
			));
		
		}
		else if (cols[0] == "COIN") {
			Coin* coin = new Coin();
			TextureManager* coinTexture = new TextureManager();
			coinTexture->initialize(entityManager.getGraphics(),
				L"./assets/coin.png");
			coin->initialize(entityManager.getGraphics(), coinTexture);
			coin->setScale(0.1);
			coin->setCenter(VECTOR2(
				std::stof(cols[1]),
				std::stof(cols[2])
			));
			entityManager.addEntity(coin, 0);
		}
		else if (cols[0] == "TURRET_SPAWNL") {
			Turret* turret = new Turret();
			TextureManager* turretTexture = new TextureManager();
			turretTexture->initialize(entityManager.getGraphics(),
				L"./assets/turret.png");
			turret->initialize(entityManager.getGraphics(), turretTexture);
			turret->setScale(0.1);
			turret->setCenter(VECTOR2(
				std::stof(cols[1]),
				std::stof(cols[2])
			));
			turret->getSprite()->flipHorizontal(TRUE);
			entityManager.addEntity(turret, 0);
		}
		else if (cols[0] == "TURRET_SPAWNR") {
			Turret* turret = new Turret();
			TextureManager* turretTexture = new TextureManager();
			turretTexture->initialize(entityManager.getGraphics(),
				L"./assets/turret.png");
			turret->initialize(entityManager.getGraphics(), turretTexture);
			turret->setScale(0.1);
			turret->setCenter(VECTOR2(
				std::stof(cols[1]),
				std::stof(cols[2])
			));
			entityManager.addEntity(turret, 0);
		}

		else if (cols[0] == "SPAWN_HUNTER") {
			Enemy* enemy = new Enemy();
			TextureManager* enemyTexture = new TextureManager();
			enemyTexture->initialize(entityManager.getGraphics(),
				L"./assets/enemy.jpg");
			enemy->initialize(entityManager.getGraphics(), enemyTexture, player);
			enemy->setCenter(VECTOR2(
				std::stof(cols[1]),
				std::stof(cols[2])
			));
			entityManager.addEntity(enemy, 0);
		}
		else if (cols[0] == "SPIKES") {
			Spikes* spikes = new Spikes();
			TextureManager* spikesTexture = new TextureManager();
			spikesTexture->initialize(entityManager.getGraphics(),
				L"./assets/spikes.png");
			spikes->initialize(entityManager.getGraphics(), spikesTexture);
			spikes->setScale(0.2);
			spikes->setCenter(VECTOR2(
				std::stof(cols[1]),
				std::stof(cols[2])
			));
			entityManager.addEntity(spikes, 0);
		}
		else if (cols[0] == "GOAL") {
			Goal* goal = new Goal();
			TextureManager* goalTexture = new TextureManager();
			goalTexture->initialize(entityManager.getGraphics(),
				L"./assets/goal.png");
			goal->initialize(entityManager.getGraphics(), goalTexture);
			goal->setScale(0.1);
			goal->setCenter(VECTOR2(
				std::stof(cols[1]),
				std::stof(cols[2])
			));
			entityManager.addEntity(goal, 0);
		}

	}
}

std::vector<std::string> LoadScene::split_string_by(const std::string& str, char delim)
{
	auto result = std::vector<std::string>{};
	auto ss = std::stringstream{ str };

	for (std::string line; std::getline(ss, line, delim);)
		result.push_back(line);

	return result;
}
