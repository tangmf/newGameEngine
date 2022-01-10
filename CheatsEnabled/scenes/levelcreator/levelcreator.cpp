// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module:			Gameplay Programming
// Assignment1:		Cheats:Enabled
// Student Name:	
// Student No.:		
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// ===========================================================================
// Level Creator Scene Implementation
// ===========================================================================

// include specification
#include "levelcreator.h"

// ===========================================================================
// default constructor
// ===========================================================================
LevelCreatorScene::LevelCreatorScene():

	// text states
	gridText		(),
	cursorText		(),
	HUDText			(),
	gTextSize		(8),
	gCharW			(0),
	gCharH			(0),
	textOffsetX		(0),
	textOffsetY		(0),

	// mouse states
	mX				(0),
	mY				(0),
	mCol			(0),
	mRow			(0),
	curXOffset		(0),
	curYOffset		(0),

	// level states
	gridBounds		(),
	levelGrid		(),
	charObjectMap	(),
	nRows			(0),
	nCols			(0),
	cellW			(0),
	cellH			(0),
	realCellW		(0),
	realCellH		(0),
	rCenterOffsetX	(0),
	rCenterOffsetY	(0),
	pSpawnCol		(0),
	pSpawnRow		(0),

	// HUD states
	brushHUDX		(0),
	brushHUDY		(24),
	brushHUDText	(L""),
	showInfo		(TRUE),

	// working states
	cGrid			(),
	cRows			(0),
	cCols			(0),
	cRect			(),

	// timers
	bsHoldTimer		(0),
	bsExitHoldTime	(1.0f)
{
	// set grid bounds to screen bounds
	gridBounds.left		= long(WindowConstants::GAME_WIDTH	* 0.1);
	gridBounds.top		= long(WindowConstants::GAME_HEIGHT * 0.1);
	gridBounds.right	= WindowConstants::GAME_WIDTH - gridBounds.left;
	gridBounds.bottom	= WindowConstants::GAME_HEIGHT - gridBounds.top;
}

// ===========================================================================
// default destructor
// ===========================================================================
LevelCreatorScene::~LevelCreatorScene() {}


// level mapping methods

// ===========================================================================
// saves the level into a file
// ===========================================================================
void LevelCreatorScene::saveLevel()
{
	// copy level grid into cGrid to work on it
	cGrid = levelGrid;
	cCols = nCols;
	cRows = nRows;

	// initialize working states
	std::string levelString;
	std::string entityName = "WALL";
	int entX, entY, rectEdgeX, rectEdgeY;

	// while there are still walls in cGrid, find them
	while (findNextRect(
		cRect,
		std::get<0>(LevelCreatorConstants::WALL)
	)) {
		// translate grid coords to screen coords
		rectEdgeX = (int) translateGridPosX(cRect.right - cRect.left + 1) / 2;
		rectEdgeY = (int) translateGridPosY(cRect.bottom - cRect.top + 1) / 2;
		entX = (int) translateGridPosX(cRect.left, FALSE) + rectEdgeX;
		entY = (int) translateGridPosY(cRect.top, FALSE) + rectEdgeY;

		// save wall to level string
		levelString += 
			entityName + "," +
			std::to_string(entX) + "," +
			std::to_string(entY) + "," +
			std::to_string(rectEdgeX) + "," +
			std::to_string(rectEdgeY) + "\n";

		// remove wall from cGrid
		markRect(cRect, std::get<0>(LevelCreatorConstants::EMPTY_SPACE));
	}

	// load remainder of the entities 
	
	// iterate through cGrid again
	for (size_t y = 0; y < cGrid.size(); y++) {
		for (size_t x = 0; x < cGrid.at(y).size(); x++) {

			// add all entities which are not empty spaces
			if (cGrid.at(y).at(x) ==
				std::get<0>(LevelCreatorConstants::EMPTY_SPACE)) 

				// skip to next entity
				continue;

			// get entity position translated to cell center
			entX = (int) translateGridPosX(x, TRUE);
			entY = (int) translateGridPosY(y, TRUE);

			// get entity name from charObjMap
			entityName = StringUtilities::UTF8ws2s(
				charObjectMap[cGrid.at(y).at(x)].first
			);

			levelString += 
				entityName + "," + 
				std::to_string(entX) + "," + 
				std::to_string(entY) + "\n";
		}
	}

	// save level string to file
	FileUtils::writeToFile(StorageConstants::LEVEL_FILE, levelString);
}

// ===========================================================================
// locates and returns the first, largest rectangle it can find in cGrid 
// made out of character c. returns true if found, and false otherwise.
// ===========================================================================
bool LevelCreatorScene::findNextRect(
	RECT&				outRect,
	const std::wstring&	c
) {
	// initialize outRect bottom right corners to bottom right of cGrid
	outRect.right	= cCols - 1;
	outRect.bottom	= cRows - 1;

	// create flow control switch
	bool foundCorner = FALSE;


	// find top left corner of rect

	// iterate through cGrid in a scanline motion
	for (int y = 0; y < cRows; y++) {
		for (int x = 0; x < cCols; x++) {

			// find first c char and use it as the top left corner
			if (cGrid[y][x] == c) {
				outRect.left	= x;
				outRect.top		= y;

				// break out of both loops
				foundCorner = TRUE;
				break;
			}
		}

		// skip to locating bottom right corner if corner found
		if (foundCorner) break;
	}

	// if we couldn't find a corner so far, no rects remain
	if (!foundCorner) return false;


	// find bottom right corner of rect

	// continue iterating from where we left off
	for (int y = outRect.top; y <= outRect.bottom; y++) {

		// if this row has a non-c character on the left rectangle edge
		if (cGrid[y][outRect.left] != c) {

			// this row is the first "imperfect" rectangle row, so that
			// means the previous row is the actual bottom rect edge.

			// note that this also means that the right edge should have
			// been found as well, since we're iterating in a scanline
			// fashion.

			// however, there will be a bias towards taller rectangles since
			// this algorithm favours reducing rect width in exchange for
			// rectangle height

			outRect.bottom = y - 1;
			return true;
		}

		for (int x = outRect.left; x <= outRect.right; x++) {

			// if this column is a non-c character
			if (cGrid[y][x] != c) {

				// this column is the first "imperfect" rectangle column, so
				// that means the the previous columns have been c characters
				// so far. thus the actual right edge was the col before this.

				outRect.right = x - 1;
				break;
			}
		}
	}

	// rectangle bottom right is at bottom right of grid, so no rows or cols
	// have non-c characters up till the last check.
	return true;
}

// ===========================================================================
// updates the characters within the specified area in cGrid to character
// c. used to mark previous rects so they don't get counted again.
// ===========================================================================
void LevelCreatorScene::markRect(
	RECT&				rect,
	const std::wstring&	c
) {
	// iterate through specified area
	for (int y = rect.top; y <= rect.bottom; y++) {
		for (int x = rect.left; x <= rect.right; x++) {

			// update character at position to c
			cGrid.at(y).at(x) = c;
		}
	}
}


// grid control methods

// ===========================================================================
// update grid rows and columns by modifier amount, then recalculates 
// grid states.
// ===========================================================================
void LevelCreatorScene::updateGrid(
	int rMod,
	int cMod
) {
	// update col states
	nCols = max(nCols + cMod, 1);

	// handle col modifier

	// if cMod < 0, remove cMod cols from nRows (not updated) rows
	if (cMod < 0 && nCols > 1)
		for (int y = 0; y < nRows; y++)
			levelGrid[y].erase(
				std::prev(levelGrid[y].end(), cMod + 1),
				levelGrid[y].end()
			);

	// if cMod > 0, add cMod cols of empty spaces to nRows (not updated) rows
	if (cMod > 0)
		for (int y = 0; y < nRows; y++)
			levelGrid[y].insert(
				levelGrid[y].end(),
				cMod,
				std::get<0>(LevelCreatorConstants::EMPTY_SPACE)
			);


	// update row states
	nRows = max(nRows + rMod, 1);

	// handle row modifier

	// if rMod < 0, remove rMod rows from levelGrid
	if (rMod < 0 && nRows > 1)
		levelGrid.erase(
			std::prev(levelGrid.end(), rMod + 1),
			levelGrid.end()
		);

	// if rMod > 0, add rMod rows with nCol cols of empty spaces to levelGrid
	else if (rMod > 0)
		levelGrid.insert(
			levelGrid.end(),
			rMod,
			std::vector<std::wstring>(
				nCols, 
				std::get<0>(LevelCreatorConstants::EMPTY_SPACE)
			)
		);

	// recalculate grid cell sizes
	cellW = (int) (gridBounds.right - gridBounds.left) / nCols;
	cellH = (int) (gridBounds.bottom - gridBounds.top) / nRows;

	// calculate cell sizes and center offsets for actual screen size
	realCellW = float(WindowConstants::GAME_WIDTH) / nCols;
	realCellH = float(WindowConstants::GAME_HEIGHT) / nRows;
	rCenterOffsetX = realCellW / 2;
	rCenterOffsetY = realCellH / 2;

	// recalculate grid cell text offsets
	textOffsetX = (int) (cellW - gCharW) / 2;
	textOffsetY = (int) (cellH - gCharH) / 2;
}


// hud methods

// ===========================================================================
// recalculate brush hud position
// ===========================================================================
void LevelCreatorScene::updateBrushHUD()
{
	// update brush HUD text
	brushHUDText = L"Current Brush: " + charObjectMap[charBrushes.front()].first;

	// recalculate width of hud object
	int brushHUDw = HUDText.calculateWidth(brushHUDText);

	// update brush display coordinates - center brush HUD
	brushHUDX = (WindowConstants::GAME_WIDTH - brushHUDw) / 2;
}


// scene methods

// ===========================================================================
// set up scene objects during scene initialization
// ===========================================================================
void LevelCreatorScene::setup()
{
	// scene setup

	getGraphics()->setBackgroundColor(CommonColors::BLACK);


	// initialize text printers

	if (!gridText.initialize(
		getGraphics(),
		gTextSize,
		true,
		false,
		L"Terminal"
	)) throw (
		InkdotError(
			InkdotErrorType::FATAL_ERROR,
			"Error initializing grid text font!"
		)
		);
	gridText.setTextColor(CommonColors::WHITE);

	if (!cursorText.initialize(
		getGraphics(),
		10,
		false,
		false,
		L"Terminal"
	)) throw (
		InkdotError(
			InkdotErrorType::FATAL_ERROR,
			"Error initializing cursor text font!"
		)
		);
	cursorText.setTextColor(CommonColors::LIME);

	if (!HUDText.initialize(
		getGraphics(),
		12,
		true,
		false,
		L"Terminal"
	)) throw (
		InkdotError(
			InkdotErrorType::FATAL_ERROR,
			"Error initializing HUD text font!"
		)
		);
	HUDText.setTextColor(CommonColors::BLACK);
	HUDText.setHighlight(CommonColors::WHITE);
	HUDText.setHighlightPadding(2);

	// calculate character width and height
	gCharH = gridText.calculateHeight(L"A");
	gCharW = gridText.calculateWidth(L"A");

	// calculate cursor text offsets
	curXOffset = cursorText.calculateHeight(L"A") / -2;
	curYOffset = cursorText.calculateHeight(L"A") / -2;

	// populate charObj map and brushes
	for (int i = 0; i < LevelCreatorConstants::N_CHAROBJ; i++) {

		charObjectMap[std::get<0>(LevelCreatorConstants::CHAROBJECTS[i])] =
			std::make_pair(
				std::get<1>(LevelCreatorConstants::CHAROBJECTS[i]),
				std::get<2>(LevelCreatorConstants::CHAROBJECTS[i])
			);

		charBrushes.push_back(
			std::get<0>(LevelCreatorConstants::CHAROBJECTS[i])
		);
	}

	// initialize grid
	levelGrid.clear();
	nRows = 0;
	nCols = 0;

	// populate grid
	updateGrid(
		WindowConstants::GAME_HEIGHT / 20,
		WindowConstants::GAME_WIDTH / 20
	);

	// initialize player spawn to middle of grid
	pSpawnCol = (int) nCols / 2;
	pSpawnRow = (int) nRows / 2;

	// add player spawn char
	levelGrid.at(pSpawnRow).at(pSpawnCol) =
		std::get<0>(LevelCreatorConstants::PLAYER_SPAWN);

	// initialize HUD
	updateBrushHUD();

	// hide cursor
	ShowCursor(FALSE);
}

// ===========================================================================
// clean up scene objects and prepare to transit out of scene
// ===========================================================================
void LevelCreatorScene::cleanup()
{
	// cleanup scene background
	getGraphics()->setBackgroundColor(WindowConstants::BACK_COLOR);

	// undo hide cursor
	ShowCursor(TRUE);
}

// ===========================================================================
// update objects for each frame
// ===========================================================================
void LevelCreatorScene::update(
	float	prevFrameTime
) {
	// handle mouse

	// get coordinates for mouse
	mX = (int) getInput()->getMouseX();
	mY = (int) getInput()->getMouseY();

	// check mouse coordinates outside grid bounds
	if (mX <= gridBounds.left	||
		mX >= gridBounds.right	||
		mY <= gridBounds.top	||
		mY >= gridBounds.bottom)
	{
		// if any of the checks are true, mouse outside grid
		mCol = mRow = -1;
	}
	
	// else, mouse is inside grid bounds
	else
	{
		// get col and row for mouse click
		mCol = (int) (mX - gridBounds.left) / cellW;
		mRow = (int) (mY - gridBounds.top) / cellH;

		// handle mouse input if mouse in grid
		if (getInput()->getMouseLBtn())
		{
			// if current brush is the player spawn, 
			if (charBrushes.front() ==
				std::get<0>(LevelCreatorConstants::PLAYER_SPAWN)
			) {
				// set previous player spawn back to an empty space
				// (if possible)
				if (pSpawnRow < nRows && pSpawnCol < nCols)
					levelGrid.at(pSpawnRow).at(pSpawnCol) =
						std::get<0>(LevelCreatorConstants::EMPTY_SPACE);

				// set new player spawn position
				levelGrid.at(mRow).at(mCol) = charBrushes.front();

				// update player spawn to current location
				pSpawnCol = mCol;
				pSpawnRow = mRow;
			}
			else
			{
				// prevent writing on player spawn
				if(levelGrid.at(mRow).at(mCol) !=
					std::get<0>(LevelCreatorConstants::PLAYER_SPAWN)
				) 
					// update mCol, mRow to current charBrush
					levelGrid.at(mRow).at(mCol) = charBrushes.front();
			}
		}
	}

	// handle brush update
	if (getInput()->wasKeyPressed(LevelCreatorConstants::QKEY))
	{
		charBrushes.push_front(charBrushes.back());
		charBrushes.pop_back();
		updateBrushHUD();
	}
	else if (getInput()->wasKeyPressed(LevelCreatorConstants::EKEY))
	{
		charBrushes.push_back(charBrushes.front());
		charBrushes.pop_front();
		updateBrushHUD();
	}
	
	// handle grid update
	if		(getInput()->wasKeyPressed(VK_OEM_MINUS))	updateGrid(0, -1);
	else if (getInput()->wasKeyPressed(VK_OEM_PLUS))	updateGrid(0, 1);
	else if (getInput()->wasKeyPressed(VK_OEM_4))		updateGrid(-1, 0);
	else if (getInput()->wasKeyPressed(VK_OEM_6))		updateGrid(1, 0);

	// handle toggle info
	if (getInput()->wasKeyPressed(VK_TAB)) showInfo = !showInfo;

	// handle exit
	if (getInput()->wasKeyPressed(VK_ESCAPE))
	{
		// save level
		saveLevel();

		// transition back to main menu
		sceneManager->transitionToScene(SceneNames::MAIN_MENU);
	}
	
	// handle exit without saving
	if (getInput()->isKeyDown(VK_BACK))
	{
		// increment bsHoldTimer while held
		bsHoldTimer += prevFrameTime;

		// check if bsHoldTimer has reached necessary threshold to exit
		if (bsHoldTimer >= bsExitHoldTime)
		{
			// exit without saving changes
			sceneManager->transitionToScene(SceneNames::MAIN_MENU);
		}
	}
	else
	{
		// reset bsHoldTimer if not held and bsHoldTimer above 0.
		if(bsHoldTimer) bsHoldTimer = 0.0f;
	}
}

// ===========================================================================
// render sprites for scene foreground: draws over entity sprites
// ===========================================================================
void LevelCreatorScene::renderForeground()
{
	// print map data
	for (int y = 0; y < nRows; y++) {
		for (int x = 0; x < nCols; x++) {

			// configure printer according to association
			switch (charObjectMap[levelGrid.at(y).at(x)].second)
			{
			case LevelCreatorConstants::NEUTRAL:
				gridText.setTextColor(LevelCreatorConstants::NEUTRALCOL);
				break;
			case LevelCreatorConstants::ENEMY:
				gridText.setTextColor(LevelCreatorConstants::ENEMYCOL);
				break;
			case LevelCreatorConstants::PLAYER:
				gridText.setTextColor(LevelCreatorConstants::PLAYERCOL);
				break;
			case LevelCreatorConstants::PICKUP:
				gridText.setTextColor(LevelCreatorConstants::PICKUPCOL);
				break;
			default: // inclusive of NONE
				gridText.setTextColor(LevelCreatorConstants::NONECOL);
				break;
			}

			// if mouse cursor is at position, use full alpha
			if (mCol == x && mRow == y)
				gridText.setTextColor(
					gridText.getTextColor() |
					CommonColors::ALPHAMASK
				);

			// print text
			gridText.print(
				levelGrid.at(y).at(x),
				x * cellW + textOffsetX + gridBounds.left,
				y * cellH + textOffsetY + gridBounds.top
			);
		}
	}

	// render HUD - brush
	HUDText.print(brushHUDText, brushHUDX, brushHUDY);

	// render HUD - info
	if (showInfo)
	{
		// print grid information
		HUDText.print(L"GRID: " +
			std::to_wstring(nCols) + L" X " +
			std::to_wstring(nRows), 24, 50);

		// print cursor pos if within frame bounds
		if (mCol >= 0 && mRow >= 0)
			HUDText.print(L"Cursor Pos: " +
				std::to_wstring((int) translateGridPosX(mCol, TRUE)) + L"," +
				std::to_wstring((int) translateGridPosY(mRow, TRUE)), 24, 75);
		else
			HUDText.print(L"Cursor Pos: Outside", 24, 75);

		// print controls
		HUDText.print(
			L"TOGGLE INFO:   [TAB]\n"
			L"SAVE AND EXIT: [ESC]\n"
			L"EXIT, NO SAVE: [BACKSPACE] - 1s\n"
			L"NEXT BRUSH:    E\n"
			L"PREV. BRUSH:   Q\n"
			L"INCREASE ROWS: ]\n"
			L"DECREASE ROWS: [\n"
			L"INCREASE COLS: +\n"
			L"DECREASE COLS: -\n", 24, 100);
	}

	// render mouse cursor
	cursorText.print({ wchar_t(158) }, mX + curXOffset, mY + curYOffset);
}

// ===========================================================================
// releases all memory reserved for graphics objects so that the bound
// graphics device can be reset. called when the graphics device is lost.
// ===========================================================================
void LevelCreatorScene::releaseAllGraphics()
{
	// handle lost device
	gridText.onLostDevice();
	HUDText.onLostDevice();
}

// ===========================================================================
// recreates and restores all graphics objects. called after a lost
// graphics device is restored.
// ===========================================================================
void LevelCreatorScene::resetAllGraphics()
{
	// handle lost device
	gridText.onResetDevice();
	HUDText.onResetDevice();
}
