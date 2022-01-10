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

// include specification
#include "texturesHandler.h"

// ===========================================================================
// default constructor
// ===========================================================================
TexturesHandler::TexturesHandler():

	// manager states
	textureRegistry()
{}

// ===========================================================================
// default destructor
// ===========================================================================
TexturesHandler::~TexturesHandler()
{
	// delete all registered textures safely
	
	// create iterator for texture registry
	std::unordered_map<std::wstring, TextureManager*>::iterator it =
		textureRegistry.begin();

	// iterate through texture registry
	while (it != textureRegistry.end()) {

		// delete texture safely
		SAFE_DELETE(it->second);

		// increment texture registry iterator
		it++;
	}
}


// registry methods

// ===========================================================================
// register texture to texture registry
// ===========================================================================
void TexturesHandler::addTexture(
	Graphics*		g,
	std::wstring	filename,
	std::wstring	textureName
) {
	// check that texture name has not been used yet
	if (textureRegistry.count(textureName) > 0) {
		
		// if so, throw a warning and return early
		throw(
			InkdotError(
				InkdotErrorType::WARNING,
				"A texture has already been registered for " +
				StringUtilities::UTF8ws2s(textureName) +
				"!"
			)
			);
		return;
	}

	// create new texture manager object
	TextureManager* tm = new TextureManager;

	// initialize texture manager safely
	if (!tm->initialize(g, filename.c_str())) throw(
		InkdotError(
			InkdotErrorType::FATAL_ERROR,
			"Could not initialize texture for " +
			StringUtilities::UTF8ws2s(textureName) +
			"!"
		)
		);

	// add texture manager object to texture registry
	textureRegistry[textureName] = tm;
}

// ===========================================================================
// load texture from texture registry
// ===========================================================================
TextureManager* TexturesHandler::getTexture(
	std::wstring	textureName
) {
	// check that texture name has been registered
	if (textureRegistry.count(textureName) < 1) {

		// if so, throw a warning and return early
		throw(
			InkdotError(
				InkdotErrorType::WARNING,
				"A texture has not been registered for " +
				StringUtilities::UTF8ws2s(textureName) +
				"!"
			)
			);
		return NULL;
	}

	// else, retrieve and return texture
	return textureRegistry[textureName];
}


// delegate methods

// ===========================================================================
// call onLostDevice() for all registered textures
// ===========================================================================
void TexturesHandler::onLostDevice()
{
	// create iterator for texture registry
	std::unordered_map<std::wstring, TextureManager*>::iterator it =
		textureRegistry.begin();

	// iterate through texture registry
	while (it != textureRegistry.end()) {

		// call onLostDevice() for texture at iterator
		it->second->onLostDevice();

		// increment texture registry iterator
		it++;
	}
}

// ===========================================================================
// call onResetDevice() for all registered textures
// ===========================================================================
void TexturesHandler::onResetDevice()
{
	// create iterator for texture registry
	std::unordered_map<std::wstring, TextureManager*>::iterator it =
		textureRegistry.begin();

	// iterate through texture registry
	while (it != textureRegistry.end()) {

		// call onResetDevice() for texture at iterator
		it->second->onResetDevice();

		// increment texture registry iterator
		it++;
	}
}
