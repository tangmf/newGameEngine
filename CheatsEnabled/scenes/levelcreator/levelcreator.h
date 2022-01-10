// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module:			Gameplay Programming
// Assignment1:		Cheats:Enabled
// Student Name:	
// Student No.:		
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// ===========================================================================
// Level Creator Scene Specification
// ===========================================================================

#ifndef _SCENE_LEVELCREATOR_H
#define _SCENE_LEVELCREATOR_H

// include necessary headers
#include "../../inkdot/include.h"
#include "../../inkdot/src/utils/strutils.h"
#include "../../inkdot/src/utils/fileutils.h"
#include <vector>
#include <unordered_map>
#include <tuple>

// typedefs

typedef std::pair<std::wstring, int> CHAROBJINFO;


// related constructs

namespace LevelCreatorConstants {

	// typedefs
	typedef std::tuple<std::wstring, std::wstring, int> CHAROBJDEF;

	// charObject associations
	const int ENEMY		= 0b1000;
	const int PLAYER	= 0b0100;
	const int PICKUP	= 0b0010;
	const int NEUTRAL	= 0b0001;
	const int NONE		= 0b0000;

	// charObject info
	const int N_CHAROBJ	= 8;

	// charObject constants
	const CHAROBJDEF EMPTY_SPACE	{ L"+", L"EMPTY_SPACE",		NONE	};
	const CHAROBJDEF WALL			{ L"W", L"WALL",			NEUTRAL	};
	const CHAROBJDEF SPIKES			{ L"^", L"SPIKES",			ENEMY	};
	const CHAROBJDEF HUNTER_SPAWN	{ L"H", L"SPAWN_HUNTER",	ENEMY	};
	const CHAROBJDEF TURRET_SPAWNL	{ L"t", L"SPAWN_TURRET_L",	ENEMY	};
	const CHAROBJDEF TURRET_SPAWNR	{ L"T", L"SPAWN_TURRET_R",	ENEMY	};
	const CHAROBJDEF PLAYER_SPAWN	{ L"@", L"SPAWN_PLAYER",	PLAYER	};
	const CHAROBJDEF COIN			{ L"$", L"COIN",			PICKUP	};

	// charObject list
	const CHAROBJDEF CHAROBJECTS[N_CHAROBJ]
	{
		EMPTY_SPACE,
		WALL,
		SPIKES,
		HUNTER_SPAWN,
		TURRET_SPAWNL,
		TURRET_SPAWNR,
		PLAYER_SPAWN,
		COIN
	};

	// grid colors for associations
	const COLOR_ARGB ENEMYCOL	= CommonColors::RED    &CommonColors::ALPHA75;
	const COLOR_ARGB PLAYERCOL	= CommonColors::BLUE   &CommonColors::ALPHA75;
	const COLOR_ARGB PICKUPCOL	= CommonColors::YELLOW &CommonColors::ALPHA75;
	const COLOR_ARGB NEUTRALCOL = CommonColors::WHITE  &CommonColors::ALPHA75;
	const COLOR_ARGB NONECOL	= CommonColors::WHITE  &CommonColors::ALPHA25;

	// key constants
	const UCHAR QKEY	= 0x51;
	const UCHAR	EKEY	= 0x45;
}


// class specification

class LevelCreatorScene : public Scene
{
private:

	// text states

	// normal grid text printer
	Text gridText;

	// cursor text printer
	Text cursorText;

	// hud text printer
	Text HUDText;

	// grid text size
	int gTextSize;

	// grid text dimensions
	int gCharW, gCharH;

	// offset to center text in cell
	int textOffsetX, textOffsetY;


	// mouse states
	
	// mouse raw screen coordinates
	int mX, mY;

	// mouse column, row coordinates
	int mCol, mRow;

	// custom cursor text offset
	int curXOffset, curYOffset;


	// level states

	// level display bounds
	RECT gridBounds;

	// grid representation of a level
	std::vector<std::vector<std::wstring>> levelGrid;

	// object types
	std::unordered_map<std::wstring, CHAROBJINFO> charObjectMap;

	// char-switch queue
	std::deque<std::wstring> charBrushes;

	// row and column dimensions
	int nRows, nCols;

	// cell dimensions
	int cellW, cellH;

	// unscaled cell dimensions
	float realCellW, realCellH;

	// unscaled cell center offset
	float rCenterOffsetX, rCenterOffsetY;

	// player spawn coordinates
	int pSpawnCol, pSpawnRow;


	// HUD states

	// brush display coordinates
	int brushHUDX, brushHUDY;

	// brush name
	std::wstring brushHUDText;

	// toggle info HUD
	bool showInfo;


	// working states

	// container for grid operations that require a copy of level
	std::vector<std::vector<std::wstring>> cGrid;

	// dimensions for cGrid
	int cRows, cCols;

	// rect container
	RECT cRect;


	// timers

	// timer to exit without saving
	float bsHoldTimer, bsExitHoldTime;

public:

	// default constructor
	LevelCreatorScene();

	// default destructor
	~LevelCreatorScene();


	// level mapping methods

	// translates a grid position to level x, y coordinates
	VECTOR2 translateGridPos(int r, int c) 
	{ return { translateGridPosX(c), translateGridPosY(r) }; }

	// translates a grid col position to actual level x-coordinates
	float translateGridPosX(int c, bool getCenter = FALSE) 
	{ return c * realCellW + (getCenter ? rCenterOffsetX : 0); }

	// translates a grid row position to actual level y-coordinates
	float translateGridPosY(int r, bool getCenter = FALSE)
	{ return r * realCellH + (getCenter ? rCenterOffsetY : 0); }

	// saves the level into a file
	void saveLevel();

	// locates and returns the first, largest rectangle it can find in cGrid 
	// made out of character c. returns true if found, and false otherwise.
	bool findNextRect(RECT& outRect, const std::wstring& c);

	// updates the characters within the specified area in cGrid to character
	// c. used to mark previous rects so they don't get counted again.
	void markRect(RECT& rect, const std::wstring& c);


	// grid control methods

	// update grid rows and columns by modifier amount, then recalculates 
	// grid states.
	void updateGrid(int rowsModifier, int colsModifier);


	// hud methods
	
	// recalculate brush hud position
	void updateBrushHUD();


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

#endif // !_SCENE_LEVELCREATOR_H