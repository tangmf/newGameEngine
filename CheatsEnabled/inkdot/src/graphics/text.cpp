// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module:			Gameplay Programming
// Assignment1:		Cheats:Enabled
// Student Name:	Eugene Long - [Engine Designer]
// Student No.:		S10193060J
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// ===========================================================================
// InkdotEngine - Text Handler (TextDX) Implementation
// ===========================================================================
// * This is a InkdotEngine component, derived from the standard GPP engine
//   templates provided by Ms. Lee Ching Yun, modified by Eugene Long.
// ===========================================================================

// include specification
#include "text.h"

// ===========================================================================
// default constructor
// ===========================================================================
Text::Text():

	// members
	font				(NULL),
	fontHeight			(0),
	color				(CommonColors::BLACK),
	highlight			(CommonColors::ALPHA0),
	highlightPadding	(1),

	// handlers
	graphics	(NULL),

	// common calc states
	textRect		(),
	transformMatrix	(),
	rotationAngle	(0)
{
	// set default textRect to game width x height
	textRect = {
		0,
		0,
		WindowConstants::GAME_WIDTH,
		WindowConstants::GAME_HEIGHT
	};
}

// ===========================================================================
// default destructor
// ===========================================================================
Text::~Text()
{
	// release font resources
	SAFE_RELEASE(font);
}


// methods

// ===========================================================================
// initializes the Text handler object instance.
// ===========================================================================
bool Text::initialize(
	Graphics*	g,
	int			height,
	bool		bold,
	bool		italic,
	LPCWSTR		fontName
) {
	// store argument info into instance state
	graphics	= g;
	fontHeight	= height;

	// initialize font weight
	UINT weight = FW_NORMAL;

	// handle bold fonts
	if (bold) weight = FW_BOLD;

	// create DirectX font
	HRESULT res = D3DXCreateFont(
		g->getD3DDevice(),
		height,
		0,			// scale fontsize to height parameter
		weight,
		1,			// mipmap levels should always be 1
		italic,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		fontName,
		&font
	);

	// ensure font created successfully, else die gracefully
	if (FAILED(res)) return false;

	// initialize text transformation matrix
	D3DXMatrixTransformation2D(
		&transformMatrix,
		NULL,
		0.0f,
		NULL,
		NULL,
		0.0f,
		NULL
	);

	// return success status
	return true;
}

// ===========================================================================
// prints a string at the specified coordinates.
// NOTE: only call this between spriteBegin()/spriteEnd()
// ===========================================================================
int Text::print(
	const std::wstring& str,
	int					x,
	int					y,
	float				z
) {
	// ensure font has been initialized
	if (font == NULL) return fontHeight;

	// update textRect coordinates
	textRect.top	= y;
	textRect.left	= x;

	// delegate to general printer
	return print(
		str,
		textRect,
		DT_LEFT,
		z
	);
}

// ===========================================================================
// prints a string with the specified bounding box and format 
// NOTE: only call this between spriteBegin()/spriteEnd()
// ===========================================================================
int Text::print(
	const std::wstring&	str,
	RECT&				rect,
	UINT				format,
	float				z
) {
	// ensure font has been initialized
	if (font == NULL) return fontHeight;

	// limit z to 255.99f to prevent highlight from drawing over text
	if (z > 255.99f) z = 255.99f;

	// check if highlight is not fully transparent
	if ((highlight & CommonColors::ALPHAMASK) > 0) {

		// draw highlight rect "behind text"
		graphics->drawPrimitiveRect(
			float(rect.left				- highlightPadding),
			float(rect.top				- highlightPadding),
			z							+ 0.01f,
			float(calculateWidth(str)	+ 2 * highlightPadding),
			float(calculateHeight(str)	+ 2 * highlightPadding),
			highlight
		);
	}

	// init z-transformation matrix
	D3DXMATRIX zTranslate;

	// apply z translation for text
	D3DXMatrixTranslation(&zTranslate, 0.0f, 0.0f, NORMALIZED_Z(z));
	graphics->getD3DSprite()->SetTransform(&zTranslate);

	// draw text
	return font->DrawTextW(
		graphics->getD3DSprite(),	// sprite object containing string
		str.c_str(),				// string to draw
		-1,							// define string as null-terminated
		&rect,						// rect to draw the text in
		format,						// text formatting methods
		color						// text color
	);
}

// ===========================================================================
// calculate textbox width required to fit text based on current font
// ===========================================================================
int Text::calculateWidth(
	const std::wstring& str
) {
	// ensure font has been initialized
	if (font == NULL) return WindowConstants::GAME_WIDTH;

	// temp rect obj that will be automatically resized
	RECT resizeRect({ 0,0,0,0 });

	// update temp rect with new height and widths
	font->DrawTextW(
		graphics->getD3DSprite(),	// sprite object containing string
		str.c_str(),				// string to draw
		-1,							// define string as null-terminated
		&resizeRect,				// rect to draw the text in
		DT_CALCRECT,				// calculate rect bounds
		color						// text color
	);

	// return width of resized rect
	return resizeRect.right;
}

// ===========================================================================
// calculate textbox height required to fit text based on current font
// ===========================================================================
int Text::calculateHeight(
	const std::wstring& str
) {
	// ensure font has been initialized
	if (font == NULL) return WindowConstants::GAME_HEIGHT;

	// temp rect obj that will be automatically resized
	RECT resizeRect({ 0,0,0,0 });

	// update temp rect with new height and widths
	return font->DrawTextW(
		graphics->getD3DSprite(),	// sprite object containing string
		str.c_str(),				// string to draw
		-1,							// define string as null-terminated
		&resizeRect,				// rect to draw the text in
		DT_CALCRECT,				// calculate rect bounds
		color						// text color
	);
}

// ===========================================================================
// release resources on device lost
// ===========================================================================
void Text::onLostDevice()
{
	// ensure font has been initialized
	if (font == NULL) return;

	font->OnLostDevice();
}

// ===========================================================================
// restore resources on device reset
// ===========================================================================
void Text::onResetDevice()
{
	// ensure font has been initialized
	if (font == NULL) return;

	font->OnResetDevice();
}
