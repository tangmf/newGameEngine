// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module:			Gameplay Programming
// Assignment1:		Cheats:Enabled
// Student Name:	Eugene Long - [Engine Designer]
// Student No.:		S10193060J
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// ===========================================================================
// InkdotEngine - Graphics Handler Specifications
// ===========================================================================
// * This is a InkdotEngine component, derived from the standard GPP engine
//   templates provided by Ms. Lee Ching Yun, modified by Eugene Long.
// ===========================================================================

#ifndef _INKDOT_GRAPHICS_H
#define _INKDOT_GRAPHICS_H

// import minimal windows headers
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// import necessary headers
#include "../common.h"
#include "../error.h"

// include d3d9 header and libraries

// NOTE: If d3dx9.h is NOT FOUND, go to Properties > VC++ Directories and add
// $(DXSDK_DIR)Include and $(DXSDK_DIR)LIB\x86 into Include Directories and
// Library Directories respectively

#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

// directx interface aliases
#define LP_TEXTURE		LPDIRECT3DTEXTURE9
#define LP_SPRITE		LPD3DXSPRITE
#define LP_3DDEVICE		LPDIRECT3DDEVICE9
#define LP_3D			LPDIRECT3D9
#define LP_FONT			LPD3DXFONT
#define LP_VERTEXBUFFER LPDIRECT3DVERTEXBUFFER9

// graphics macros

// normalize z value according to max z depth
#define NORMALIZED_Z(z) { \
	z < 0.0f \
	? 0.0f \
	: z < GameConstants::MAX_Z_DEPTH \
	? z / GameConstants::MAX_Z_DEPTH \
	: 1 \
}

// related constructs

struct SpriteData
{
public:

	int			width;			// width of sprite in pixels
	int			height;			// height of sprite in pixels
	float		x;				// x-coord for top left corner of sprite
	float		y;				// y-coord for top left corner of sprite
	float		z;				// z-coord for depth of sprite (0 to 100)
	float		angle;			// rotation angle in radians
	float		scale;			// % size scaling for sprite
	RECT		sheetRect;		// bounds of source texture on spritesheet
	LP_TEXTURE	texture;		// pointer to texture data
	bool		flipHorizontal;	// true to flip sprite horizontally (mirror)
	bool		flipVertical;	// true to flip sprite vertically
};

struct Vertex
{
	// position components
	FLOAT	x, y, z;

	// vertex color
	COLOR_ARGB	color;

	// texture coordinates
	FLOAT	tu, tv;


	// constructor
	Vertex(
		float		ix,
		float		iy,
		float		iz,
		COLOR_ARGB	c,
		float		u,
		float		v
	):
		x		(ix),
		y		(iy),
		z		(iz),
		color	(c),
		tu		(u),
		tv		(v)
	{}
};

// define FVF format for Vertex
#define D3DFVF_Vertex (D3DFVF_XYZ | D3DFVF_DIFFUSE)


// class specification

class Graphics
{
private:

	// window states

	// handle to bound window
	HWND			hwnd;

	// window dimensions
	int				wndWidth, wndHeight;

	// window display mode
	bool			fullscreen;

	// background display color
	COLOR_ARGB		backColor;


	// directx interface pointers

	// direct3d interface
	LP_3D			direct3d;

	// d3d device interface
	LP_3DDEVICE		device3d;

	// d3d sprite interface
	LP_SPRITE		sprite3d;


	// buffers

	// shared vertex buffer pointer
	LP_VERTEXBUFFER g_pVB;


	// misc.

	// d3d presentation parameters
	D3DPRESENT_PARAMETERS d3dpp;

	// common result container
	HRESULT res;


	// methods

	// initialize direct3d presentation parameters
	// (for internal engine use only. no user-serviceable parts inside)
	void initD3Dpp();

public:

	// default constructor
	Graphics();

	// default destructor
	~Graphics();

	// safely release all pointer referenced objects
	void releaseAll();


	// methods

	/// <summary>
	/// Initializes the Graphics handler object instance.
	/// </summary>
	/// <param name="hwnd">
	/// : Handle to the window to bind the graphics handler to.
	/// </param>
	/// <param name="width">
	/// : Width of the window in pixels
	/// </param>
	/// <param name="height">
	/// : Height of the window in pixels
	/// </param>
	/// <param name="fullscreen"></param>
	void initialize(
		HWND	hwnd,
		int		width,
		int		height,
		bool	fullscreen
	);

	// clear backbuffer and begin directx scene draw sequence
	HRESULT beginSceneDraw();

	// end directx scene draw sequence
	HRESULT endSceneDraw();

	// display offscreen buffer on the screen (page flip)
	HRESULT showBackBuffer();

	// returns the current state of the graphics device
	HRESULT getDeviceState();

	// resets the graphics device
	HRESULT	resetDevice();


	// sprite drawing

	// create and save a texture from a file to a texture object
	HRESULT loadTexture(
		LPCWSTR			filename,
		COLOR_ARGB		transcolor,
		UINT&			width,
		UINT&			height,
		LP_TEXTURE&		texture
	);

	// draws a sprite as described in a SpriteData struct
	void drawSprite(
		const SpriteData&	spriteData,
		COLOR_ARGB			color
	);

	// begin sprite drawing sequence
	void beginSpriteDraw()	
	{
		sprite3d->Begin(
			D3DXSPRITE_ALPHABLEND | 
			D3DXSPRITE_SORT_DEPTH_FRONTTOBACK
		); 
	}

	// end sprite drawing sequence
	void endSpriteDraw()
	{ sprite3d->End(); }


	// primitive drawing

	/// <summary>
	/// Draws a filled primitive rectangle with the specified bounds
	/// </summary>
	/// <param name="x">
	/// : The top-left x-coordinate of the rect to draw
	/// </param>
	/// <param name="y">
	/// : The top-left y-coordinate of the rect to draw
	/// </param>
	/// <param name="w">
	/// : The width of the rect to draw in pixels
	/// </param>
	/// <param name="h">
	/// : The height of the rect to draw in pixels
	/// </param>
	/// <param name="z">
	/// : The top-left z-coordinate of the rect to draw
	/// </param>
	/// <param name="startCol">
	/// : The starting color of the gradient rect fill (or the entire rect if
	///   no endCol is specified)
	/// </param>
	/// <param name="endCol">
	/// : The ending color of the gradient rect fill
	/// </param>
	/// <param name="verticalGradient">
	/// : Set this to true for a top-down gradient, or false for a left-right
	///   gradient.
	/// </param>
	void drawPrimitiveRect(
		const float&	x,
		const float&	y,
		const float&	z,
		const float&	w,
		const float&	h,
		COLOR_ARGB		startCol,
		COLOR_ARGB		endCol				= NULL,
		bool			verticalGradient	= TRUE
	);

	/// <summary>
	/// Draws a primitive with the specified vertices
	/// </summary>
	/// <param name="v">
	/// : An array of vertices to draw
	/// </param>
	/// <param name="vertices">
	/// : The number of vertices in v
	/// </param>
	/// <param name="type">
	/// : The type of primitive to draw
	/// </param>
	/// <param name="primitiveCount">
	/// : The number of primitives to draw
	/// </param>
	void drawPrimitive(
		Vertex				v[],
		int					nVerts,
		D3DPRIMITIVETYPE	type,
		int					primitiveCount
	);


	// getters

	// return direct3d interface
	LP_3D		getD3D()		{ return direct3d; }

	// return d3d device interface
	LP_3DDEVICE	getD3DDevice()	{ return device3d; }

	// return d3d sprite interface
	LP_SPRITE	getD3DSprite()	{ return sprite3d; }

	// return handle to device context for the bound window
	HDC			getDC()			{ return GetDC(hwnd); }

	// return handle to window
	HWND		getHWND()		const { return hwnd; }

	// return pointer to vertex buffer pointer
	LP_VERTEXBUFFER* getPtrPVB() { return &g_pVB; }


	// setters

	// set background display color
	void setBackgroundColor(COLOR_ARGB c) { backColor = c; }

};

#endif // !_INKDOT_GRAPHICS_H