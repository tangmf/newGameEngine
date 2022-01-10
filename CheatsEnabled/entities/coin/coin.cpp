// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module:			Gameplay Programming
// Assignment1:		Cheats:Enabled
// Student Name:	Tang Ming Feng
// Student No.:		S10185023E
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// ===========================================================================
// InkdotEngine - Coin Entity Class Implementation
// ===========================================================================

// include specifications
#include "coin.h"

//============================================================================
// default constructor
//============================================================================
Coin::Coin() :

	// base object constructor
	InkdotLegacyEntity()
{}

//============================================================================
// default destructor
//============================================================================
Coin::~Coin() {}


// entity methods

//============================================================================
// initializes the entity - called on scene initialization
//============================================================================
void Coin::initialize(
	Graphics*		g,
	TextureManager*	tm
) {
	// initialize base object
	InkdotLegacyEntity::initialize(
		g,
		CoinConstants::WIDTH,
		CoinConstants::HEIGHT,
		CoinConstants::TEXTURE_COLS,
		tm,
		CoinConstants::INIT_X,
		CoinConstants::INIT_Y,
		CoinConstants::MASS
	);

	// customize collider
	getCollider("sprite")->changeToCircleCollider(
		CoinConstants::COLLIDER_RADIUS
	);

	// add coin tag
	addTag("coin");
}
