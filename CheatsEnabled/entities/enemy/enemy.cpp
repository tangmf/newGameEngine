// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module:			Gameplay Programming
// Assignment1:		Cheats:Enabled
// Student Name:	Tang Ming Feng
// Student No.:		S10185023E
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// ===========================================================================
// InkdotEngine - Enemy Entity Class Implementation
// ===========================================================================

// include specifications
#include "enemy.h"

//============================================================================
// default constructor
//============================================================================
Enemy::Enemy():
	playerDetect(),
	player(),

	// base object constructor
	InkdotLegacyEntity()
{}

//============================================================================
// default destructor
//============================================================================
Enemy::~Enemy() {}


// entity methods

//============================================================================
// initializes the entity - called on scene initialization
//============================================================================
void Enemy::initialize(
	Graphics*		g,
	TextureManager*	tm,
	Player* playerptr
) {
	// initialize base object
	InkdotLegacyEntity::initialize(
		g,
		EnemyConstants::WIDTH,
		EnemyConstants::HEIGHT,
		EnemyConstants::TEXTURE_COLS,
		tm,
		EnemyConstants::INIT_X,
		EnemyConstants::INIT_Y,
		EnemyConstants::MASS
	);

	// configure sprite animation
	sprite.setFrames(EnemyConstants::START_FRAME, EnemyConstants::END_FRAME);
	sprite.setCurrentFrame(EnemyConstants::START_FRAME);
	sprite.setFrameDelay(EnemyConstants::ANIM_DELAY);
	sprite.setLoop(1);
	player = playerptr;
}

//============================================================================
// updates the entity - called each frame before scene update() starts
//============================================================================
void Enemy::onUpdateEnd(float prevFrameTime)
{
	if (player && abs(player->getCenter()->x - this->getCenter()->x) <= 100 && abs(player->getCenter()->y - this->getCenter()->y) <= 100) {
		if (player->getCenter()->x - this->getCenter()->x > 0) {
			this->getSprite()->flipHorizontal(FALSE);
		}
		else {
			this->getSprite()->flipHorizontal(TRUE);
		}
		setVelocity(VECTOR2(player->getCenter()->x - this->getCenter()->x,getVelocity()->y));
	}

	// bounce off walls
	if (int w = getCollider("sprite")->outsideRect(WindowConstants::BOUNDS))
	{
		switch (w)
		{
		case 1: if (velocity.x < 0) setVelocityX(-velocity.x); break;
		case 2: if (velocity.y < 0) setVelocityY(-velocity.y); break;
		case 3: if (velocity.x > 0) setVelocityX(-velocity.x); break;
		case 4: if (velocity.y > 0) setVelocityY(0); break;
		}
	}

	if (this->collidesWithTaggedEntities(
		"sprite",
		"player",
		collisionList
	)) {

		InkdotEntity* e;

		// handle each collision in collision list
		for (size_t i = 0; i < collisionList.size(); i++) {

			e = entityManager->getEntity(std::get<2>(collisionList[i]));
			// kill player
			e->setActive(FALSE);
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
				setVelocityX(0);
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

//void Enemy::targetPlayer(InkdotEntity player, float prevFrameTime) {
//	setVelocity(-VECTOR2(player.getCenter()->x, player.getCenter()->y) + VECTOR2(getCenter()->x, getCenter()->y));
//}
