// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module:			Gameplay Programming
// Assignment1:		Cheats:Enabled
// Student Name:	Eugene Long - [Engine Designer]
// Student No.:		S10193060J
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// ===========================================================================
// Texture Handler Class Specification
// ===========================================================================
// * This is a InkdotEngine component, derived from the standard GPP engine
//   templates provided by Ms. Lee Ching Yun, modified by Eugene Long.
// ===========================================================================

#ifndef _INKDOT_TEXTURESHANDLER_H
#define _INKDOT_TEXTURESHANDLER_H

// include necessary headers
#include "textureManager.h"
#include "../utils/strutils.h"
#include <unordered_map>
#include <string>

// class specification

class TexturesHandler
{
private:

	// manager states
	
	// string to texture map
	std::unordered_map<std::wstring, TextureManager*> textureRegistry;

public:

	// default constructor
	TexturesHandler();

	// default destructor
	~TexturesHandler();

	
	// registry methods

	// register texture to texture registry
	void addTexture(
		Graphics*		g,
		std::wstring	filename, 
		std::wstring	textureName
	);

	// load texture from texture registry
	TextureManager* getTexture(std::wstring textureName);


	// delegate methods

	// calls onLostDevice() for all registered textures
	void onLostDevice();

	// valls onResetDevice() for all registered textures
	void onResetDevice();

};

#endif // !_INKDOT_TEXTURESHANDLER_H