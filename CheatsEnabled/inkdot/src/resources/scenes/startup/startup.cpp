// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module:			Gameplay Programming
// Assignment1:		Cheats:Enabled
// Student Name:	Eugene Long - [Engine Designer]
// Student No.:		S10193060J
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// ===========================================================================
// InkdotEngine - Engine Startup Scene Implementation
// ===========================================================================
// * This is a InkdotEngine component, derived from the standard GPP engine
//   templates provided by Ms. Lee Ching Yun, modified by Eugene Long.
// ===========================================================================

// include specification
#include "startup.h"

// ===========================================================================
// default constructor
// ===========================================================================
StartupScene::StartupScene():
	
	// scene objects
	titleText	(),
	titleText2	(),

	// working states
	titleW		(0),
	nTitleCols	(0),
	nTitleRows	(0),
	colSpacing	(7),
	rowSpacing	(10),
	xOffset		(0),
	yOffset		(0),
	hTitleCol	(0),
	hTitleRow	(0),
	sceneTimer	(4.0f),
	cellW		(0),
	cellH		(0),
	colorIndex	(0),

	// wave states
	waveTimer		(0),
	waveStepTime	(0.005f),
	waveC			(0),
	waveWidth		(7),
	maxWaveN		(6),
	waveOffsets		(5),
	initStep		(0),
	currStep		(0),
	maxStep			(0),
	wavesLeft		(3)
{
	// populate color palette
	palette[0] = SETCOLOR_ARGB(255,  34,  17,  80);
	palette[1] = SETCOLOR_ARGB(255,  95,  24, 127);
	palette[2] = SETCOLOR_ARGB(255, 152,  45, 128);
	palette[3] = SETCOLOR_ARGB(255, 211,  67, 110);
	palette[4] = SETCOLOR_ARGB(255, 248, 118,  92);
	palette[5] = SETCOLOR_ARGB(255, 254, 187, 129);
	palette[6] = SETCOLOR_ARGB(255, 252, 253, 191);
}


// logic methods

// ===========================================================================
// returns the result of the wave function w, which is always guaranteed
// to be within the integer range -1 < w(c, r) < maxWaveN
// ===========================================================================
int StartupScene::w(
	int c,
	int r
) {
	return (int) maxWaveN - abs(((r + c)) / waveWidth - currStep - waveC);
}


// abstract scene methods

// ===========================================================================
// set up scene objects during scene initialization
// ===========================================================================
void StartupScene::setup()
{
	// scene setup

	getGraphics()->setBackgroundColor(CommonColors::BLACK);


	// initialize fonts

	if (!titleText.initialize(
		getGraphics(),
		13,
		true,
		false,
		L"Terminal"
	)) throw (
		InkdotError(
			InkdotErrorType::FATAL_ERROR,
			"Error initializing startup title font!"
		)
	);

	titleText.setTextColor(CommonColors::WHITE & CommonColors::ALPHA25);

	if (!titleText2.initialize(
		getGraphics(),
		13,
		true,
		false,
		L"Terminal"
	)) throw (
		InkdotError(
			InkdotErrorType::FATAL_ERROR,
			"Error initializing startup title font 2!"
		)
		);

	titleText2.setTextColor(CommonColors::BLACK);
	titleText2.setHighlight(CommonColors::WHITE);
	titleText2.setHighlightPadding(3);

	// perform initial calculations

	titleW = titleText.calculateWidth(L"inkdot:engine");

	// get window dimensions
	RECT clientRect;
	GetClientRect(getGraphics()->getHWND(), &clientRect);

	// calculate cell dimensions
	cellW = titleW + colSpacing;
	cellH = 13 + rowSpacing;

	// get number of title texts required to fill width of screen
	nTitleCols = clientRect.right / cellW + 2;
	nTitleRows = clientRect.bottom / cellH + 2;

	// calculate offsets required to center text display vertically
	xOffset = ((nTitleCols * cellW) - clientRect.right) / 2;

	// choose a random col and row to highlight
	srand((unsigned int) time(NULL));
	hTitleCol = rand() % (nTitleCols - 4) + 2;
	hTitleRow = rand() % (nTitleRows - 4) + 2;

	// calculate constants for wave function
	waveC		= ((nTitleCols + nTitleRows) >> 1) / waveWidth;
	currStep	= initStep = (maxWaveN + waveC) * -1 - waveOffsets;
	maxStep		= initStep * -1 + waveOffsets;
}

// ===========================================================================
// clean up scene objects and prepare to transit out of scene
// ===========================================================================
void StartupScene::cleanup()
{
	// reset background color
	getGraphics()->setBackgroundColor(WindowConstants::BACK_COLOR);
}

// ===========================================================================
// update objects for each frame
// ===========================================================================
void StartupScene::update(float prevFrameTime)
{
	// decrement scene timer
	sceneTimer -= prevFrameTime;

	// if scene time is up or any key pressed, transition to initial scene
	if (sceneTimer <= 0 || getInput()->anyKeyPressed()) {
		sceneManager->transitionToScene(SceneNames::INITIAL_GAME_SCENE);
	}

	// increment wave timer
	if (wavesLeft > 0) waveTimer += prevFrameTime;

	// if timer passes step time, increment wave step
	if (waveTimer >= waveStepTime) {

		// increment wave step
		if (currStep < maxStep) currStep++;
		else
		{
			// reset wave
			currStep = initStep;
			wavesLeft--;
		}

		// reset timer
		waveTimer = 0;
	}
}

// ===========================================================================
// handle object rendering for each object on each frame
// ===========================================================================
void StartupScene::renderForeground()
{
	for (int r = 0; r < nTitleRows; r++) {
		for (int c = 0; c < nTitleCols; c++) {

			// calculate color for row, col
			colorIndex = w(c, r);
			if (colorIndex >= 0)
				getGraphics()->drawPrimitiveRect(
					(float) c * cellW - xOffset,
					(float) r * cellH,
					GameConstants::MAX_Z_DEPTH,
					(float) cellW,
					(float) cellH,
					palette[colorIndex]
				);

			// draw title texts
			if (r == hTitleRow && c == hTitleCol)
			{
				if (colorIndex > 2) {
					// switch to dark bg for contrast
					titleText2.setTextColor(CommonColors::WHITE);
					titleText2.setHighlight(CommonColors::BLACK);
				}
				titleText2.print(
					L"inkdot:engine",
					c * cellW - xOffset,
					r * cellH
				);
				if (colorIndex > 2) {
					// switch back
					titleText2.setTextColor(CommonColors::BLACK);
					titleText2.setHighlight(CommonColors::WHITE);
				}
			}
			else
				titleText.print(
					L"inkdot:engine",
					c * cellW - xOffset,
					r * cellH
				);
		}
	}
}

// ===========================================================================
// releases all memory reserved for graphics objects so that the bound
// graphics device can be reset. called when the graphics device is lost.
// ===========================================================================
void StartupScene::releaseAllGraphics()
{
	// release graphics objects
	titleText.onLostDevice();
	titleText2.onLostDevice();
}

// ===========================================================================
// recreates and restores all graphics objects. called after a lost
// graphics device is restored.
// ===========================================================================
void StartupScene::resetAllGraphics()
{
	// reset graphics objects
	titleText.onResetDevice();
	titleText2.onLostDevice();
}