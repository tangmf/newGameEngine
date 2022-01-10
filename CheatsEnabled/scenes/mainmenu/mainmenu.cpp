// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module:			Gameplay Programming
// Assignment1:		Cheats:Enabled
// Student Name:	Eugene Long - [Engine Designer]
// Student No.:		S10193060J
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// ===========================================================================
// Main Menu Scene Implementation
// ===========================================================================

// include specification
#include "mainmenu.h"

// ===========================================================================
// default constructor
// ===========================================================================
MainMenuScene::MainMenuScene():
	
	// background states
	menuBGText		(),
	bgXPos			(0),
	bgYPos			(0),
	bgContent		(L""),
	charCols		(0),
	charRows		(0),
	bgCharSet		(L"@#$%&?*+-"),
	refreshDelay	(1/24.0f),
	refreshTimer	(0),

	// title states
	menuTitleText1		(),
	menuTitleText2		(),
	titleStr1			(L"Cheats"),
	titleStr2			(L":Enabled"),
	titleSpacing		(12),
	title1XPos			(0),
	title1YPos			(0),
	title2XPos			(0),
	title2YPos			(0),
	title2Highlighted	(TRUE),
	title2HighlightCol	(CommonColors::LIME),
	titleBlinkDelay		(1.0f),
	titleBlinkTimer		(0),

	// menu states
	menuText			(),
	options				(),
	optionSpacing		(20),
	prefixStr			(L">"),
	selectedMenuIndex	(0),

	// working states
	windowW			(0),
	windowH			(0)
{}


// shared methods

// ===========================================================================
// initialize shared or common states
// ===========================================================================
void MainMenuScene::initializeSharedStates()
{
	// get window bounds

	RECT clientRect;
	GetClientRect(getGraphics()->getHWND(), &clientRect);

	windowW = clientRect.right;
	windowH = clientRect.bottom;
}


// dynamic scene bg methods

// ===========================================================================
// set up scene objects during scene initialization
// ===========================================================================
void MainMenuScene::initializeBackground()
{
	// initialize bg text

	if (!menuBGText.initialize(
		getGraphics(),
		10,
		false,
		false,
		L"Terminal"
	)) throw (
		InkdotError(
			InkdotErrorType::FATAL_ERROR,
			"Failed to initialize menu bg text!"
		)
	);

	menuBGText.setTextColor(SETCOLOR_ARGB(40, 255, 255, 255));


	// perform bg calculations

	// calculate fixed-width character dimensions
	int charW = menuBGText.calculateWidth(L"A");
	int charH = menuBGText.calculateHeight(L"A");

	// calculate characters required to fill screen
	charCols = windowW / charW;
	charRows = windowH / charH;

	// seed the randomiser function
	srand((unsigned int)time(NULL));

	// calculate offsets to center bgContent
	bgXPos = (windowW % charW) / 2;
	bgYPos = (windowH % charH) / 2;
}

// ===========================================================================
// update background text
// ===========================================================================
void MainMenuScene::updateBackground(
	float	prevFrameTime
) {
	// update timer
	refreshTimer += prevFrameTime;

	// if refresh timer is not ready yet, return early
	if (refreshTimer < refreshDelay) return;

	// clear content back to empty string
	bgContent.clear();

	// fill bgContent
	for (int r = 0; r < charRows; r++) {
		for (int c = 0; c < charCols; c++) {
			bgContent += bgCharSet[rand() % bgCharSet.length()];
		}
		if (r != charRows) bgContent += L'\n';
	}

	// reset timer
	refreshTimer = 0;
}

// ===========================================================================
// draw background (should be called during sprite draw sequence)
// ===========================================================================
void MainMenuScene::renderBackground()
{
	menuBGText.print(
		bgContent,
		bgXPos,
		bgYPos
	);
}


// scene title methods

// ===========================================================================
// initialize title states
// ===========================================================================
void MainMenuScene::initializeTitle()
{
	// initialize menu title text

	if (!menuTitleText1.initialize(
		getGraphics(),
		28,
		true,
		false,
		L"Terminal"
	)) throw (
		InkdotError(
			InkdotErrorType::FATAL_ERROR,
			"Failed to initialize menu title text 1!"
		)
	);

	menuTitleText1.setTextColor(CommonColors::WHITE);

	if (!menuTitleText2.initialize(
		getGraphics(),
		28,
		true,
		false,
		L"Terminal"
	)) throw (
		InkdotError(
			InkdotErrorType::FATAL_ERROR,
			"Failed to initialize menu title text 2!"
		)
	);

	menuTitleText2.setTextColor(CommonColors::BLACK);
	menuTitleText2.setHighlight(title2HighlightCol);
	menuTitleText2.setHighlightPadding(3);


	// calculate offsets to position menu text

	int title1W = menuTitleText1.calculateWidth(titleStr1);
	int title2W = menuTitleText2.calculateWidth(titleStr2);

	title1XPos = (windowW - title1W - title2W - titleSpacing) / 2;
	title2XPos = title1XPos + title1W + titleSpacing;
	title1YPos = title2YPos = (int)(0.3f * windowH);
}

// ===========================================================================
// update title states
// ===========================================================================
void MainMenuScene::updateTitle(
	float	prevFrameTime
) {
	// update timer
	titleBlinkTimer += prevFrameTime;

	// if refresh timer is not ready yet, return early
	if (titleBlinkTimer < titleBlinkDelay) return;

	// toggle highlights
	title2Highlighted = !title2Highlighted;

	// update title highlights accordingly
	if (title2Highlighted) {

		// turn on highlight
		menuTitleText2.setTextColor(CommonColors::BLACK);
		menuTitleText2.setHighlight(title2HighlightCol);
	}
	else {

		// turn off highlight
		menuTitleText2.setTextColor(title2HighlightCol);
		menuTitleText2.setHighlight(CommonColors::ALPHA0);
	}

	// reset timer
	titleBlinkTimer = 0;
}

// ===========================================================================
// draw title (should be called during sprite draw sequence)
// ===========================================================================
void MainMenuScene::renderTitle()
{
	// draw title parts

	menuTitleText1.print(
		titleStr1,
		title1XPos,
		title1YPos
	);

	menuTitleText2.print(
		titleStr2,
		title2XPos,
		title2YPos
	);
}


// menu methods

// ===========================================================================
// initialize menu states
// ===========================================================================
void MainMenuScene::initializeMenu()
{
	// initialize menu text

	if (!menuText.initialize(
		getGraphics(),
		22,
		false,
		false,
		L"Terminal"
	)) throw (
		InkdotError(
			InkdotErrorType::FATAL_ERROR,
			"Failed to initialize menu text!"
		)
	);

	menuText.setTextColor(CommonColors::WHITE);
	menuText.setHighlightPadding(3);

	// register menu options

	options.push_back(MenuOption(L"begin", &menuText));
	options.push_back(MenuOption(L"test scene", &menuText));
	options.push_back(MenuOption(L"level creator", &menuText));
	options.push_back(MenuOption(L"quit", &menuText));
	

	options[selectedMenuIndex].selected = true;

	// calculate positions for each option

	int optionHeight = menuText.print(L"A", NULL, DT_CALCRECT);
	int totalMenuHeight =
		options.size() * (optionHeight + optionSpacing) - optionSpacing;
	int startYPos = (windowH - totalMenuHeight) / 2;

	for (size_t i = 0; i < options.size(); i++) {
		options[i].xPos = (windowW - options[i].labelWidth) / 2;
		options[i].yPos = (startYPos + (i * (optionHeight + optionSpacing)));
	}
}

// ===========================================================================
// update menu selection states
// ===========================================================================
void MainMenuScene::updateSelectedStates(
	int indexModifier
) {
	int pIndex = selectedMenuIndex;
	selectedMenuIndex += indexModifier;

	// ensure index is not negative
	if (selectedMenuIndex < 0) selectedMenuIndex = options.size() - 1;

	// ensure index does not exceed range of options
	// negative sign is used to flip sign bit for unsigned comparison
	else if ((size_t) -selectedMenuIndex > options.size()) 
		selectedMenuIndex %= options.size();

	// if selection did not change, return early.
	if (selectedMenuIndex == pIndex) return;

	// update option states accordingly
	options[pIndex]				.selected = false;
	options[selectedMenuIndex]	.selected = true;
}

// ===========================================================================
// update menu states
// ===========================================================================
void MainMenuScene::updateMenu(
	float	prevFrameTime
) {
	// handle user input

	if (getInput()->wasKeyPressed(VK_UP)) {
		updateSelectedStates(-1);
	}
	else if (getInput()->wasKeyPressed(VK_DOWN)) {
		updateSelectedStates(1);
	}
	else if (getInput()->wasKeyPressed(VK_RETURN)) {

		// handle selection
		switch (selectedMenuIndex) {

		// 0: start game
		case 0: sceneManager->transitionToScene("LOAD_SCENE"); break;

		// 1: test scene
		case 1: sceneManager->transitionToScene(SceneNames::SCENE1); break;

		// 2: level creator
		case 2: sceneManager->transitionToScene(SceneNames::LEVELCREATOR); break;

		// 3: quit game
		case 3: sceneManager->transitionToScene(SceneNames::EXIT_GAME); break;

		
		}
		
	}
}

// ===========================================================================
// draw menu (should be called during sprite draw sequence)
// ===========================================================================
void MainMenuScene::renderMenu()
{
	// render each option in the menu
	for (size_t i = 0; i < options.size(); i++) {
		options[i].print();
	}
}

// scene methods

// ===========================================================================
// set up scene objects during scene initialization
// ===========================================================================
void MainMenuScene::setup()
{
	// configure scene

	getGraphics()->setBackgroundColor(CommonColors::BLACK);


	// initialize scene objects

	initializeSharedStates();
	initializeBackground();
	initializeTitle();
	initializeMenu();
	
}

// ===========================================================================
// clean up scene objects and prepare to transit out of scene
// ===========================================================================
void MainMenuScene::cleanup()
{
	// reset bg color
	getGraphics()->setBackgroundColor(WindowConstants::BACK_COLOR);

	// clear options
	options.clear();
}

// ===========================================================================
// update objects for each frame
// ===========================================================================
void MainMenuScene::update(
	float	prevFrameTime
) {
	// update dynamic bg
	updateBackground(prevFrameTime);

	// update title
	updateTitle(prevFrameTime);

	// update menu
	updateMenu(prevFrameTime);
}

// ===========================================================================
// handle sprite rendering for each object on each frame
// ===========================================================================
void MainMenuScene::renderForeground()
{
	renderTitle();
	renderMenu();
}

// ===========================================================================
// releases all memory reserved for graphics objects so that the bound
// graphics device can be reset. called when the graphics device is lost.
// ===========================================================================
void MainMenuScene::releaseAllGraphics()
{
	// bg
	menuBGText.onLostDevice();

	// title
	menuTitleText1.onLostDevice();
	menuTitleText2.onLostDevice();

	// menu
	menuText.onLostDevice();
}

// ===========================================================================
// recreates and restores all graphics objects. called after a lost
// graphics device is restored.
// ===========================================================================
void MainMenuScene::resetAllGraphics()
{
	// bg
	menuBGText.onResetDevice();

	// title
	menuTitleText1.onResetDevice();
	menuTitleText2.onResetDevice();

	// menu
	menuText.onResetDevice();
}
