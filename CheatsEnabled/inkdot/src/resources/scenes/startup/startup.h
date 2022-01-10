// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module:			Gameplay Programming
// Assignment1:		Cheats:Enabled
// Student Name:	Eugene Long - [Engine Designer]
// Student No.:		S10193060J
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// ===========================================================================
// InkdotEngine - Engine Startup Scene Specification
// ===========================================================================
// * This is a InkdotEngine component, derived from the standard GPP engine
//   templates provided by Ms. Lee Ching Yun, modified by Eugene Long.
// ===========================================================================

#ifndef _INKDOT_SCENE_STARTUP_H
#define _INKDOT_SCENE_STARTUP_H

// import necessary headers
#include "../../../game/scene.h"
#include "../../../common.h"
#include <time.h>

// class specification

class StartupScene : public Scene
{
private:

	// scene objects

	// viridis color palette array
	COLOR_ARGB palette[7];

	// startup title text printer
	Text titleText;
	Text titleText2;

	// working states

	// title width
	int titleW;

	// number of columns and rows of title text to generate
	int nTitleCols, nTitleRows;

	// row and col spacings
	int colSpacing, rowSpacing;

	// offsets
	int xOffset, yOffset;

	// highlighted title text pos
	int hTitleCol, hTitleRow;

	// time till startupScene dismisses itself (in seconds)
	float sceneTimer;

	// cell dimensions
	int cellW, cellH;

	// color index shared working state
	int colorIndex;


	// wave states

	// timer for wave step increments
	float waveTimer;

	// controls speed of wave - delay until wave step is incremented
	float waveStepTime;

	// wave constant 
	int waveC;

	// wave width (of continuous colored cells)
	int waveWidth;

	// maximum wave value cap
	int maxWaveN;

	// wave step offsets - affects time until next wave activates
	int waveOffsets;

	// initial wave step - wave is to maximum left offset
	int initStep;

	// current wave step
	int currStep;

	// maximum wave step - wave is to maximum right offset
	int maxStep;

	// number of waves left to present to user
	int wavesLeft;

public:

	// default constructor
	StartupScene();

	// default destructor
	~StartupScene() {}


	// logic methods

	// returns the result of the wave function w, which is always guaranteed
	// to be within the integer range -1 < w(c, r) < maxWaveN
	int w(int c, int r);


	// scene methods

	// deallocate all manually allocated scene objects and reset object to the
	// pre-initialized state. will be called on instance destruction.
	virtual void deleteAll() {};


	// abstract scene methods

	// set up scene objects during scene initialization
	virtual void setup();

	// clean up scene objects and prepare to transit out of scene
	virtual void cleanup();

	// update objects for each frame
	virtual void update(float prevFrameTime);

	// handle artificial intelligence on each frame
	virtual void ai() {}

	// handle collisions for objects on each frame
	virtual void collisions() {}

	// render sprites for scene foreground: draws over entity sprites
	virtual void renderForeground();

	// render sprites for scene background: draws under entity sprites
	virtual void renderBackground() {}

	// releases all memory reserved for graphics objects so that the bound
	// graphics device can be reset. called when the graphics device is lost.
	virtual void releaseAllGraphics();

	// recreates and restores all graphics objects. called after a lost
	// graphics device is restored.
	virtual void resetAllGraphics();
};

#endif // !_INKDOT_SCENE_STARTUP_H