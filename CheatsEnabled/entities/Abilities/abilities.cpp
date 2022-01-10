// Pro// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module:			Gameplay Programming
// Assignment1:		Cheats:Enabled
// Student Name:	Josiah Low
// Student No.:		S10204535J
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// ===========================================================================
// InkdotEngine - Ability Entity Class Implementation
// ===========================================================================

// include specification
#include "abilities.h"

//============================================================================
// default constructor
//============================================================================
Abilities::Abilities() :

	// base object constructor
	InkdotLegacyEntity()
{}

//============================================================================
// default destructor
//============================================================================
Abilities::~Abilities() {}


// entity methods

//=============================================================================
// initializes the entity - called on scene initialization
//=============================================================================
void Abilities::initialize(
	Graphics* g,
	TextureManager* tm
) {
	// initialize base object
	InkdotLegacyEntity::initialize(
		g,
		AbilitiesConstants::WIDTH,
		AbilitiesConstants::HEIGHT,
		AbilitiesConstants::TEXTURE_COLS,
		tm,
		AbilitiesConstants::INIT_X,
		AbilitiesConstants::INIT_Y
	);
}

//=============================================================================
// updates the entity - called each frame before scene update() starts
//=============================================================================
//void Abilities::update(float	prevFrameTime)
//{
//}

