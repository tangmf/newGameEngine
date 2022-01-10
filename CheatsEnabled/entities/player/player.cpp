// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module:			Gameplay Programming
// Assignment1:		Cheats:Enabled
// Student Name:	Josiah Low
// Student No.:		S10204535J
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// ===========================================================================
// InkdotEngine - Enemy Entity Class Implementation
// ===========================================================================

// include specifications
#include "player.h"

//============================================================================
// default constructor
//============================================================================
Player::Player() :
	grounded(),
	coincount(),

	// base object constructor
	InkdotLegacyEntity()
{}

//============================================================================
// default destructor
//============================================================================
Player::~Player() {}


// entity methods

//============================================================================
// initializes the entity - called on scene initialization
//============================================================================
void Player::initialize(
	Graphics* g,
	TextureManager* tm
) {
	// initialize base object
	InkdotLegacyEntity::initialize(
		g,
		PlayerConstants::WIDTH,
		PlayerConstants::HEIGHT,
		PlayerConstants::TEXTURE_COLS,
		tm,
		PlayerConstants::INIT_X,
		PlayerConstants::INIT_Y,
		PlayerConstants::MASS
	);
	addCollider("groundcheck", 0, PlayerConstants::HEIGHT * scale / 2, {-20,-5,20,5});
	// configure sprite animation
	sprite.setFrames(PlayerConstants::START_FRAME, PlayerConstants::END_FRAME);
	sprite.setCurrentFrame(PlayerConstants::START_FRAME);
	sprite.setFrameDelay(PlayerConstants::ANIM_DELAY);
	sprite.setLoop(1);

	// add tag player
	addTag("player");
}

//============================================================================
// updates the entity - called each frame before scene update() starts
//============================================================================
void Player::onUpdateEnd(float prevFrameTime)
{


	setVelocityX(getVelocity()->x * 0.9);
	// player control
	if (getInput()->isKeyDown(0x41))
	{
		/*enemy2.flipHorizontal(1);
		enemy2.setDeltaV(VECTOR2(-enemyNS::SPEED, 0));*/
		this->sprite.flipHorizontal(TRUE);
		setVelocityX(-PlayerConstants::SPEED*prevFrameTime);
	}

	else if (getInput()->isKeyDown(0x44))
	{
		/*enemy2.flipHorizontal(0);
		enemy2.setDeltaV(VECTOR2(enemyNS::SPEED, 0));*/

		this->sprite.flipHorizontal(FALSE);

		setVelocityX(PlayerConstants::SPEED * prevFrameTime);
	}

	else if (getInput()->isKeyDown(0x57) && grounded)
	{
		/*enemy2.setVelocity(enemy2.getVelocity()
		+ VECTOR2(0, -enemyNS::SPEED * 4));*/

		setVelocityY(-PlayerConstants::SPEED* 3 * prevFrameTime);
	}

	// bounce off walls
	if (int w = getCollider("sprite")->outsideRect(WindowConstants::BOUNDS))
	{
		switch (w)
		{
		case 1: if (velocity.x < 0) setVelocityX(-velocity.x); break;
		case 2: if (velocity.y < 0) setVelocityY(-velocity.y); break;
		case 3: if (velocity.x > 0) setVelocityX(-velocity.x); break;
		case 4: if (velocity.y > 0) setVelocityY(0); grounded = true; break;
		}
	}
	else if (this->collidesWithTaggedEntities(
		"groundcheck",
		"WALL",
		collisionList
	))
	{
		grounded = true;
	}


	else {
		grounded = false;
	}

	if (this->collidesWithTaggedEntities(
		"sprite",
		"coin",
		collisionList
	)) {

		InkdotEntity* e;

		// handle each collision in collision list
		for (size_t i = 0; i < collisionList.size(); i++) {

			e = entityManager->getEntity(std::get<2>(collisionList[i]));
			// kill coin
			e->setActive(FALSE);
			// add coincount
			coincount += 1;

			//enemy2.setActive(0);
			//enemy2.setVisible(0);
		}
	}

	if (this->collidesWithTaggedEntities(
		"sprite",
		"WALL",
		collisionList
	)) {

		InkdotEntity* e;

		// handle each collision in collision list
		for (size_t i = 0; i < collisionList.size(); i++) {

			e = entityManager->getEntity(std::get<2>(collisionList[i]));

			if (std::get<0>(collisionList[i]).x * getVelocity()->x > 0) { // same direction
				if (std::get<0>(collisionList[i]).x < 0) {

				}
				else if (std::get<0>(collisionList[i]).x > 0) {

				}
				else {
					setVelocityX(0);
				}
				
			}
			else {
				
			}
			if (std::get<0>(collisionList[i]).y * getVelocity()->y > 0) { // same direction
				setVelocityY(0);
			}
			else {
				
			}

			
		}
	}

	if (this->collidesWithTaggedEntities(
		"sprite",
		"turret",
		collisionList
	)) {

		InkdotEntity* e;

		// handle each collision in collision list
		for (size_t i = 0; i < collisionList.size(); i++) {

			e = entityManager->getEntity(std::get<2>(collisionList[i]));

			if (std::get<0>(collisionList[i]).x * getVelocity()->x > 0) { // same direction
				if (std::get<0>(collisionList[i]).x < 0) {

				}
				else if (std::get<0>(collisionList[i]).x > 0) {

				}
				else {
					setVelocityX(0);
				}

			}
			else {

			}
			if (std::get<0>(collisionList[i]).y * getVelocity()->y > 0) { // same direction
				setVelocityY(0);
			}
			else {

			}


		}
	}





	// apply entity velocity to its position
	applyVelocity(prevFrameTime);

	// call base method
	InkdotLegacyEntity::onUpdateEnd(prevFrameTime);
}
