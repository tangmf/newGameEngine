// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module:			Gameplay Programming
// Assignment1:		Cheats:Enabled
// Student Name:	Eugene Long - [Engine Designer]
// Student No.:		S10193060J
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// ===========================================================================
// InkdotEngine - Image Handler Specification
// ===========================================================================
// * This is a InkdotEngine component, derived from the standard GPP engine
//   templates provided by Ms. Lee Ching Yun, modified by Eugene Long.
// ===========================================================================

#ifndef _INKDOT_IMAGE_H
#define _INKDOT_IMAGE_H

// import minimal windows headers
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// import necessary headers
#include "textureManager.h"
#include "../common.h"


// class specification

class Image
{
protected:

	// members

	// sprite data used to draw the image (via Graphics::drawSprite)
	SpriteData	spriteData;

	// image color filter (use WHITE for no filter)
	COLOR_ARGB	colorFilter;

	// flag specifying if image should be visible
	bool		visible;

	
	// animation states

	// number of columns in the spritesheet
	int		sheetCols;

	// first frame of image animation ("Z"-shape order)
	int		startFrame;

	// last frame of image animation ("Z"-shape order)
	int		endFrame;

	// current frame of image animation ("Z"-shape order)
	int		currentFrame;

	// length of time between each animation frame in seconds
	float	frameDelay;

	// time elapsed since last animation frame change
	float	animationTimer;

	// flag specifying if animation should loop
	bool	loop;

	// flag specifying if animation is complete and loop is disabled
	bool	animationCompleted;


	// handlers

	// pointer to graphics handler drawing this image
	Graphics* graphics;

	// pointer to texture manager providing texture data for this image
	TextureManager* textureManager;

	
	// misc.

	// flag specifying if object has been initialized
	bool	initialized;

	// common result container
	HRESULT	hr;

public:

	// default constructor
	Image();

	// default destructor
	~Image();


	// methods

	/// <summary>
	/// Initializes the Image object
	/// </summary>
	/// <param name="g">
	/// : Pointer to the Graphics handler instance to bind this image to
	/// </param>
	/// <param name="width">
	/// : Width of the sprite in pixels (set as 0 to use full texture width)
	/// </param>
	/// <param name="height">
	/// : Height of the sprite in pixels (set as 0 to use full texture height)
	/// </param>
	/// <param name="nCols">
	/// : Number of columns in the spritesheet
	/// </param>
	/// <param name="tm">
	/// : The TextureManager instance to source texture data from
	/// </param>
	/// <returns>
	/// : True if the image is successfully initialized and false otherwise
	/// </returns>
	bool initialize(
		Graphics*			g,
		int					width,
		int					height,
		int					nCols,
		TextureManager*		tm
	);

	// update spriteData.sheetRect according to current animation frame index
	void updateSheetRect();

	// draws the image using a color filter (defaults to WHITE = no filter)
	void draw(
		COLOR_ARGB color = CommonColors::WHITE
	);

	// draws the image using the specified sprite data. the current sprite's
	// source rect is used to specify the source rect for the new sprite data.
	void draw(
		SpriteData	sd,
		COLOR_ARGB	color = CommonColors::WHITE
	);

	// updates the animation while using frameTime to regulate the speed
	void updateAnimation(float frameTime);


	// convenience methods

	// flips the image horizontally (mirrored on y-axis)
	void flipHorizontal(bool flip)	{ spriteData.flipHorizontal = flip; }

	// flips the image vertically (mirrored on x-axis)
	void flipVertical(bool flip)	{ spriteData.flipVertical = flip; }


	// getters

	// return reference to SpriteData structure.
	const SpriteData& getSpriteInfo() 
	{ return spriteData; }

	// return image visibility status.
	bool  getVisible()
	{ return visible; }

	// return x position (top-left).
	float getX()
	{ return spriteData.x; }

	// return y position (top-left).
	float getY()
	{ return spriteData.y; }

	// return z position (render depth).
	float getZ()
	{ return spriteData.z; }

	// return scale factor.
	float getScale()
	{ return spriteData.scale; }

	// return width.
	int   getWidth()
	{ return spriteData.width; }

	// return height.
	int   getHeight()
	{ return spriteData.height; }

	// return x position (center).
	float getCenterX()
	{ return spriteData.x + spriteData.width / 2 * getScale(); }

	// return y position (center).
	float getCenterY()
	{ return spriteData.y + spriteData.height / 2 * getScale(); }

	// return rotation angle in degrees.
	float getDegrees()
	{ return spriteData.angle * (180.0f / (float) GameConstants::PI); }

	// return rotation angle in radians.
	float getRadians()
	{ return spriteData.angle; }

	// return delay between frames of animation.
	float getFrameDelay()
	{ return frameDelay; }

	// return number of starting frame.
	int   getStartFrame()
	{ return startFrame; }

	// return number of ending frame.
	int   getEndFrame()
	{ return endFrame; }

	// return number of current frame.
	int   getCurrentFrame()
	{ return currentFrame; }

	// return image's position on spritesheet as a rect.
	RECT  getSpriteDataRect()
	{ return spriteData.sheetRect; }

	// return animation completed status.
	bool  getAnimationComplete()
	{ return animationCompleted; }

	// return currently applied color filter.
	COLOR_ARGB getColorFilter()
	{ return colorFilter; }


	// setters

	// set current frame of animation
	void setCurrentFrame(int c)
	{
		// ensure frame is valid
		if (c < 0) return;

		currentFrame = c;
		animationCompleted = false;

		// update sprite rect for current frame
		updateSheetRect();
	}

	// set x position (top-left).
	void setX(float newX)
	{ spriteData.x = newX; }

	// set y position (top-left).
	void setY(float newY)
	{ spriteData.y = newY; }

	// set z position (render depth - higher values are away from the camera).
	void setZ(float newZ)
	{ spriteData.y = newZ; }

	// set scale factor
	virtual void setScale(float s)
	{ spriteData.scale = s; }

	// set rotation angle in degrees
	// 0 degrees is up. angles progress clockwise
	void setDegrees(float deg)
	{ spriteData.angle = deg * ((float) GameConstants::PI / 180.0f); }

	// set rotation angle in radians
	// 0 radians is up. angles progress clockwise
	void setRadians(float rad)
	{ spriteData.angle = rad; }

	// set image visibility
	void setVisible(bool v)
	{ visible = v; }

	// set delay between frames of animation
	void setFrameDelay(float d)
	{ frameDelay = d; }

	// set starting and ending frames of animation
	void setFrames(int s, int e)
	{ startFrame = s; endFrame = e; }

	// set image's position on spritesheet
	void setSpriteDataRect(RECT r)
	{ spriteData.sheetRect = r; }

	// set animation loop state
	void setLoop(bool lp)
	{ loop = lp; }

	// set animation completed status
	void setAnimationComplete(bool a)
	{ animationCompleted = a; };

	// set currently applied color filter (use WHITE for no change)
	void setColorFilter(COLOR_ARGB color)
	{ colorFilter = color; }

	// set TextureManager instance bound to this object
	void setTextureManager(TextureManager* textureM)
	{ textureManager = textureM; }

};

#endif // !_INKDOT_IMAGE_H
