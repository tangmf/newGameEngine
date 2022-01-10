// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module:			Gameplay Programming
// Assignment1:		Cheats:Enabled
// Student Name:	Eugene Long - [Engine Designer]
// Student No.:		S10193060J
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// ===========================================================================
// InkdotEngine - Image Handler Implementation
// ===========================================================================
// * This is a InkdotEngine component, derived from the standard GPP engine
//   templates provided by Ms. Lee Ching Yun, modified by Eugene Long.
// ===========================================================================

// include specification
#include "image.h"

// ===========================================================================
// default constructor
// ===========================================================================
Image::Image():

	// members
	spriteData	(),
	colorFilter	(CommonColors::WHITE),
	visible		(TRUE),

	// animation states
	sheetCols			(1),
	startFrame			(0),
	endFrame			(0),
	currentFrame		(0),
	frameDelay			(1.0f),
	animationTimer		(0.0f),
	loop				(FALSE),
	animationCompleted	(FALSE),

	// handlers
	graphics		(NULL),
	textureManager	(NULL),

	// misc
	initialized		(FALSE),
	hr				(NULL)
{
	// initialize spriteData properties
	spriteData.width			= 2;
	spriteData.height			= 2;
	spriteData.x				= 0.0f;
	spriteData.y				= 0.0f;
	spriteData.z				= GameConstants::MAX_Z_DEPTH / 2.0f;
	spriteData.scale			= 1.0f;
	spriteData.angle			= 0.0f;
	spriteData.flipHorizontal	= FALSE;
	spriteData.flipVertical		= FALSE;
	spriteData.texture			= NULL;

	// initialize spritesheet source rect
	spriteData.sheetRect.left		= 0;
	spriteData.sheetRect.top		= 0;
	spriteData.sheetRect.right		= spriteData.width;
	spriteData.sheetRect.bottom		= spriteData.height;
}

// ===========================================================================
// default destructor
// ===========================================================================
Image::~Image() {}


// methods

// ===========================================================================
// initializes the Image object
// ===========================================================================
bool Image::initialize(
	Graphics*			g,
	int					width,
	int					height,
	int					nCols,
	TextureManager*		tm
) {
	// attempt to initialize image
	try
	{
		// store argument info into instance state
		graphics		= g;
		textureManager	= tm;
		sheetCols		= nCols;

		// attempt to get texture
		spriteData.texture = textureManager->getTexture();

		// default to full width of texture if width is 0
		if (width == 0) width = textureManager->getWidth();

		// default to full height of texture if height is 0
		if (height == 0) height = textureManager->getHeight();

		// update stored spriteData parameters
		spriteData.width	= width;
		spriteData.height	= height;

		// if sheetCols is 0, set it to 1 to allow division
		if (sheetCols == 0) sheetCols = 1;

		// update sprite rect for current frame
		updateSheetRect();
	}
	catch (...)
	{
		// handle errors
		return false;
	}

	// up
	initialized = true;
	return true;
}

// ===========================================================================
// update spriteData.sheetRect according to current animation frame index
// ===========================================================================
void Image::updateSheetRect()
{
	// update spriteData.sheetRect to draw currentFrame
	spriteData.sheetRect.left 
		= (currentFrame % sheetCols) * spriteData.width;
	spriteData.sheetRect.top 
		= (currentFrame / sheetCols) * spriteData.height;

	// right edge and bottom edge +1
	spriteData.sheetRect.right 
		= spriteData.sheetRect.left + spriteData.width;
	spriteData.sheetRect.bottom 
		= spriteData.sheetRect.top + spriteData.height;
}

// ===========================================================================
// draws the image using a color filter (defaults to WHITE - no filter)
// ===========================================================================
void Image::draw(
	COLOR_ARGB	color
) {
	// ensure image is visible and graphics handler is available
	if (!visible || graphics == NULL) return;

	// get fresh texture in case onReset() was called
	spriteData.texture = textureManager->getTexture();

	// handle filters and draw sprite accordingly
	if (color == CommonColors::FILTER)
		graphics->drawSprite(spriteData, colorFilter);
	else
		graphics->drawSprite(spriteData, color);
}

// ===========================================================================
// draws the image using the specified sprite data. the current sprite's
// location on the spritesheet is used to select the new texture.
// ===========================================================================
void Image::draw(
	SpriteData	sd,
	COLOR_ARGB	color
) {
	// ensure image is visible and graphics handler is available
	if (!visible || graphics == NULL) return;

	// use the current Image rect to select texture
	sd.sheetRect = spriteData.sheetRect;

	// get fresh texture in case onReset() was called
	sd.texture = textureManager->getTexture();

	// handle filters and draw sprite accordingly
	if (color == CommonColors::FILTER)
		graphics->drawSprite(sd, colorFilter);
	else
		graphics->drawSprite(sd, color);
}

// ===========================================================================
// updates the animation while using frameTime to regulate the speed
// ===========================================================================
void Image::updateAnimation(
	float	frameTime
) {
	// ensure sprite is animated
	if (endFrame - startFrame <= 0) return;

	// update total elapsed time
	animationTimer += frameTime;

	// if animation timer is still lower than frame delay, continue waiting
	if (animationTimer <= frameDelay) return;

	// else, update animation

	// update timer and frame counter
	animationTimer -= frameDelay;
	currentFrame++;

	// update frames
	if (currentFrame < startFrame || currentFrame > endFrame) {

		// handle looping animations
		if (loop == true) currentFrame = startFrame;

		// else if non-looping, complete animation
		else {
			currentFrame = endFrame;
			animationCompleted = true;
		}
	}

	// update sprite rect for current frame
	updateSheetRect();
}