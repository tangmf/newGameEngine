// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module:			Gameplay Programming
// Assignment1:		Cheats:Enabled
// Student Name:	Tang Ming Feng
// Student No.:		S10185023E
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// ===========================================================================
// InkdotEngine - Turret Entity Class Implementation
// ===========================================================================

// include specification
#include "turret.h"

//============================================================================
// default constructor
//============================================================================
Turret::Turret():

	// base object constructor
	InkdotLegacyEntity(),

	// entity states
	shotTimer		(),
	cooldownTime	(2.0f),

	// bullet states
	bulletTexture	(),
	bulletList		()
{}

//============================================================================
// default destructor
//============================================================================
Turret::~Turret()
{
	// deallocate all bullets in object pool when turret is dealloc'd
	for (size_t i = 0; i < bulletList.size(); i++) {

		// attempt to delete bullet
		SAFE_DELETE(bulletList[i]);
	}
}


// game logic methods

//============================================================================
// shoots a bullet
//============================================================================
void Turret::shoot(
	Bullet* bullet,
	float	x,
	float	y,
	bool	flip
) {
	bullet->setCenter(VECTOR2(x, y));
	bullet->setActive(TRUE);
	bullet->setVelocity(VECTOR2(0, 0));
	if (flip) {
		bullet->getSprite()->flipHorizontal(TRUE);
		bullet->setDeltaV(VECTOR2(-BulletConstants::SPEED, 0));
	}
	else {
		bullet->getSprite()->flipHorizontal(FALSE);
		bullet->setDeltaV(VECTOR2(BulletConstants::SPEED, 0));
	}
}


// entity methods

//============================================================================
// initializes the entity - called on scene initialization
//============================================================================
void Turret::initialize(
	Graphics*		g,
	TextureManager* tm
) {
	// initialize base object
	InkdotLegacyEntity::initialize(
		g,
		TurretConstants::WIDTH,
		TurretConstants::HEIGHT,
		TurretConstants::TEXTURE_COLS,
		tm,
		TurretConstants::INIT_X,
		TurretConstants::INIT_Y,
		TurretConstants::MASS
	);

	// bullet
	if (!bulletTexture.initialize(
		g,
		L"./assets/bullet.png"
	))
		throw(
			InkdotError(
				InkdotErrorType::FATAL_ERROR,
				"Error initializing bullet texture!"
			));
	addTag("turret");
}

//============================================================================
// updates the entity - called each frame before scene update() starts
//============================================================================
void Turret::onUpdateEnd(
	float	prevFrameTime
) {
	// bounce off walls
	if (int w = getCollider("sprite")->outsideRect(WindowConstants::BOUNDS))
	{
		switch (w)
		{
		case 1: if (velocity.x < 0) setVelocityX(-1/velocity.x); break;
		case 2: if (velocity.y < 0) setVelocityY(-1/velocity.y); break;
		case 3: if (velocity.x > 0) setVelocityX(-1/velocity.x); break;
		case 4: if (velocity.y > 0) setVelocityY(-1/velocity.y); break;
		}
	}

	// shooting logic

	// update shot timer
	shotTimer += prevFrameTime;

	// check if shot timer has passed cooldown time
	if (shotTimer >= cooldownTime) {

		// index of a bullet from bulletList which can be shot (not active)
		// if this is -1, that means that no bullets are available.
		int availableBulletIndex = -1;

		// check for available bullets
		for (size_t i = 0; i < bulletList.size(); i++) {
			if (!bulletList[i]->isActive()) availableBulletIndex = i;
		}

		// if no bullet available, create a new bullet and add it to the pool
		if (availableBulletIndex == -1) {

			// create a new bullet
			Bullet* b = new Bullet();
			b->initialize(getGraphics(), &bulletTexture);
			b->setScale(0.1f);
			b->setActive(FALSE);
			b->addTag("bullet");
			
			// add bullet to bullet list
			bulletList.push_back(b);

			// register bullet to entity manager
			entityManager->addEntity(b);

			// use recent bullet
			availableBulletIndex = bulletList.size() - 1;
		}
		
		// shoot bullet
		shoot(bulletList[availableBulletIndex], center.x, center.y, TRUE);

		// reset shot timer
		shotTimer = 0;
	}

	// apply entity velocity to its position
	applyVelocity(prevFrameTime);
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
	// call base method
	InkdotLegacyEntity::onUpdateEnd(prevFrameTime);
}

//============================================================================
// releases all memory reserved for graphics objects so that the bound
// graphics device can be reset - called when the graphics device is lost
//============================================================================
void Turret::releaseAllGraphics()
{
	bulletTexture.onLostDevice();
}

//============================================================================
// recreates and restores all graphics objects - called when the lost graphics
// device is restored
//============================================================================
void Turret::resetAllGraphics()
{
	bulletTexture.onResetDevice();
}
