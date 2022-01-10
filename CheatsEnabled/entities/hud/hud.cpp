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
#include "hud.h"

//============================================================================
// default constructor
//============================================================================
Hud::Hud() :

	// base object constructor
	InkdotLegacyEntity()
{}

//============================================================================
// default destructor
//============================================================================
Hud::~Hud() {}


// entity methods

//=============================================================================
// initializes the entity - called on scene initialization
//=============================================================================
void Hud::initialize(
	Graphics* g,
	TextureManager* tm
) {
	// initialize base object
	InkdotLegacyEntity::initialize(
		g,
		HudConstants::WIDTH,
		HudConstants::HEIGHT,
		HudConstants::TEXTURE_COLS,
		tm,
		HudConstants::INIT_X,
		HudConstants::INIT_Y
	);
}

//=============================================================================
// updates the entity - called each frame before scene update() starts
//=============================================================================
void Hud::update(float	prevFrameTime)
{
}

