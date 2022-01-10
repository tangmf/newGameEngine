// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module:			Gameplay Programming
// Assignment1:		Cheats:Enabled
// Student Name:	Eugene Long - [Engine Designer]
// Student No.:		S10193060J
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// ===========================================================================
// InkdotEngine - Text Handler (TextDX) Specification
// ===========================================================================
// * This is a InkdotEngine component, derived from the standard GPP engine
//   templates provided by Ms. Lee Ching Yun, modified by Eugene Long.
// ===========================================================================

#ifndef _INKDOT_TEXT_H
#define _INKDOT_TEXT_H

// import minimal windows headers
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// import necessary headers
#include "graphics.h"
#include "../common.h"
#include <string>


// class specification

class Text
{
private:

	// members

	// d3d font data
	LP_FONT		font;

	// font height
	int			fontHeight;

	// text color
	COLOR_ARGB	color;

	// highlight
	COLOR_ARGB	highlight;

	// highlight padding
	int			highlightPadding;


	// handlers

	// pointer to graphics handler
	Graphics*	graphics;


	// common calculation states

	// working rect for textbox boundaries
	RECT		textRect;

	// working matrix for textbox transformation
	D3DXMATRIX	transformMatrix;

	// working float for textbox rotation angle
	float		rotationAngle;

public:

	// default constructor
	Text();

	// default destructor
	~Text();


	// methods

	/// <summary>
	/// Initializes the Text handler object instance.
	/// </summary>
	/// <param name="gPtr">
	/// : The graphics handler to bind this instance to.
	/// </param>
	/// <param name="height">
	/// : The height to use for each character. Scales font size accordingly
	/// </param>
	/// <param name="bold">
	/// : Specifies if the text should be bolded.
	/// </param>
	/// <param name="italic">
	/// : Specifies if the text should be italicized.
	/// </param>
	/// <param name="fontName">
	/// : Specifies the font family to use.
	/// </param>
	/// <returns>
	/// True if initialization succeeds and false otherwise.
	/// </returns>
	bool initialize(
		Graphics*	g,
		int			height,
		bool		bold,
		bool		italic,
		LPCWSTR		fontName
	);

	/// <summary>
	/// Prints a string at the specified coordinates.
	/// NOTE: Only call this between spriteBegin()/spriteEnd()
	/// </summary>
	/// <param name="str">
	/// : The string to print.
	/// </param>
	/// <param name="x">
	/// : The x-coordinate to print the string at.
	/// </param>
	/// <param name="y">
	/// : The y-coordinate to print the string at.
	/// </param>
	/// <param name="z">
	/// : The render depth of the string.
	/// </param>
	/// <returns>
	/// : The height of the text when printing succeeds and 0 otherwise.
	/// </returns>
	int print(
		const std::wstring& str,
		int					x,
		int					y,
		float				z	= GameConstants::MAX_Z_DEPTH / 4
	);

	/// <summary>
	/// Prints a string with the specified bounding box and format 
	/// NOTE: Only call this between spriteBegin()/spriteEnd()
	/// </summary>
	/// <param name="str">
	/// : The string to print.
	/// </param>
	/// <param name="rect">
	/// : A rect specifying the bounding box to draw the text within
	/// </param>
	/// <param name="format">
	/// : Specifies text formatting methods to use when drawing the text.
	/// </param>
	/// <param name="z">
	/// : The render depth of the string.
	/// <returns>
	/// : The height of the text when printing succeeds and 0 otherwise.
	/// </returns>
	int print(
		const std::wstring&	str,
		RECT&				rect,
		UINT				format,
		float				z	= GameConstants::MAX_Z_DEPTH / 4
	);

	// calculate textbox width required to fit text based on current font
	int calculateWidth(const std::wstring& str);

	// calculate textbox height required to fit text based on current font
	int calculateHeight(const std::wstring& str);

	// release resources on device lost
	void onLostDevice();

	// restore resources on device reset
	void onResetDevice();


	// getters

	// return rotation angle in degrees.
	virtual float getDegrees()
	{ return rotationAngle * (180.0f / (float) GameConstants::PI); }

	// return rotation angle in radians.
	virtual float getRadians()
	{ return rotationAngle; }

	// returns the current text color
	virtual COLOR_ARGB getTextColor()
	{ return color; }


	// setters

	// set rotation angle in degrees.
	// 0 degrees is up. angles progress clockwise.
	virtual void setDegrees(float deg)
	{ rotationAngle = deg * ((float) GameConstants::PI / 180.0f); }

	// set rotation angle in radians.
	// 0 radians is up. angles progress clockwise.
	virtual void setRadians(float rad)
	{ rotationAngle = rad; }

	// set text color. use the SETCOLOR_ARGB macro or CommonColors:: colors
	virtual void setTextColor(COLOR_ARGB c)
	{ color = c; }

	// set text highlight. use the SETCOLOR_ARGB macro or CommonColors::colors
	virtual void setHighlight(COLOR_ARGB c)
	{ highlight = c; }

	virtual void setHighlightPadding(int padding)
	{ highlightPadding = padding; }
};

#endif // !_INKDOT_TEXT_H