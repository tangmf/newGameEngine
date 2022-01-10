// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module:			Gameplay Programming
// Assignment1:		Cheats:Enabled
// Student Name:	Eugene Long - [Engine Designer]
// Student No.:		S10193060J
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// ===========================================================================
// InkdotEngine - Texture Manager Specification
// ===========================================================================
// * This is a InkdotEngine component, derived from the standard GPP engine
//   templates provided by Ms. Lee Ching Yun, modified by Eugene Long.
// ===========================================================================

#ifndef _INKDOT_TEXTUREMANAGER_H
#define _INKDOT_TEXTUREMANAGER_H

// import minimal windows headers
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// import necessary headers
#include "graphics.h"
#include "../common.h"


// class specification

class TextureManager
{
private:

	// members

	// width of texture in pixels
	UINT		width;

	// height of texture in pixels
	UINT		height;

	// pointer to texture resource
	LP_TEXTURE	texture;

	// name of file containing texture
	LPCWSTR		filename;


	// handlers

	// pointer to graphics handler
	Graphics*	graphics;


	// misc.

	// flag specifying if object has been initialized
	bool		initialized;

	// common result container
	HRESULT		hr;

public:

	// default constructor
	TextureManager();

	// default destructor
	~TextureManager();


	// methods

	/// <summary>
	/// Initializes the TextureManager object instance
	/// </summary>
	/// <param name="g">
	/// : Pointer to the Graphics handler instance to bind the texture to
	/// </param>
	/// <param name="file">
	/// : The name of the image file to load the texture resource from
	/// </param>
	/// <returns>
	/// True if the file is loaded successfully and false otherwise
	/// </returns>
	bool initialize(
		Graphics*	g,
		LPCWSTR		filename
	);

	// release resources on device lost
	void onLostDevice();

	// restore resources on device reset
	void onResetDevice();


	// getters

	// returns a pointer to the texture resource
	LP_TEXTURE getTexture()	const { return texture; }

	// returns the texture's width
	UINT getWidth()			const { return width; }

	// returns the texture's height
	UINT getHeight()		const { return height; }

};

#endif // !_INKDOT_TEXTUREMANAGER_H
