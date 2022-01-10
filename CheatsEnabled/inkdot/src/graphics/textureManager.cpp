// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module:			Gameplay Programming
// Assignment1:		Cheats:Enabled
// Student Name:	Eugene Long - [Engine Designer]
// Student No.:		S10193060J
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// ===========================================================================
// InkdotEngine - Texture Manager Implementation
// ===========================================================================
// * This is a InkdotEngine component, derived from the standard GPP engine
//   templates provided by Ms. Lee Ching Yun, modified by Eugene Long.
// ===========================================================================

// include specification
#include "textureManager.h"

// ===========================================================================
// default constructor
// ===========================================================================
TextureManager::TextureManager():

	// members
	width		(0),
	height		(0),
	texture		(NULL),
	filename	(NULL),

	// handlers
	graphics	(NULL),

	// misc
	initialized	(FALSE),
	hr			(NULL)
{}

// ===========================================================================
// default destructor
// ===========================================================================
TextureManager::~TextureManager()
{
	SAFE_RELEASE(texture);
}


// methods

// ===========================================================================
// initializes the textureManager object instance
// ===========================================================================
bool TextureManager::initialize(
	Graphics*	g,
	LPCWSTR		filename
) {
	// attempt to load textures from file
	try {

		// store argument info into instance state
		this->graphics = g;
		this->filename = filename;

		// load texture from file
		hr = graphics->loadTexture(
			filename,
			CommonColors::TRANS_KEY,
			width,
			height,
			texture
		);

		// ensure texture loads successfully
		if (FAILED(hr)) {

			// fail gracefully and exit early
			SAFE_RELEASE(texture);
			return false;
		}
	}
	catch (...)
	{
		// handle errors
		return false;
	}

	// if successful, update initialization flag
	initialized = true;
	return true;
}

// ===========================================================================
// release resources on device lost
// ===========================================================================
void TextureManager::onLostDevice()
{
	// ensure texture manager was initialized in the first place
	if (!initialized) return;

	// if so, release texture gracefully
	SAFE_RELEASE(texture);
}

// ===========================================================================
// restore resources on device reset
// ===========================================================================
void TextureManager::onResetDevice()
{
	// ensure texture manager was initialized in the first place
	if (!initialized) return;

	// if so, attempt to reload textures
	graphics->loadTexture(
		filename,
		CommonColors::TRANS_KEY,
		width,
		height,
		texture
	);
}
