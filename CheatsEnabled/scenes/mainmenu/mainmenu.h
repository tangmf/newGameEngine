// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module:			Gameplay Programming
// Assignment1:		Cheats:Enabled
// Student Name:	Eugene Long - [Engine Designer]
// Student No.:		S10193060J
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// ===========================================================================
// Main Menu Scene Specification
// ===========================================================================

#ifndef _SCENE_MAINMENU_H
#define _SCENE_MAINMENU_H

// import necessary headers
#include "../../inkdot/include.h"
#include <time.h>
#include <vector>


// related structures

struct MenuOption {

	// option label
	std::wstring label;

	// label text printer pointer
	Text* labelText;

	// label width
	int labelWidth;

	// position on screen
	int xPos, yPos;

	// selected flag
	bool selected;


	// constructor
	MenuOption(std::wstring l, Text* pText):
		label			(l),
		labelText		(pText),
		xPos			(0),
		yPos			(0),
		selected		(FALSE)
	{
		// calculate label width using text printer
		labelWidth = labelText->calculateWidth(label);
	}

	// print at stored coordinates
	void print()
	{
		if (selected) {
			labelText->setTextColor(CommonColors::BLACK);
			labelText->setHighlight(CommonColors::WHITE);
		}

		labelText->print(
			label,
			xPos,
			yPos
		);

		if (selected) {
			labelText->setTextColor(CommonColors::WHITE);
			labelText->setHighlight(CommonColors::ALPHA0);
		}
	}
};


// class specification

class MainMenuScene : public Scene
{
private:

	// background states

	// menu background text printer
	Text menuBGText;

	// bg text offsets
	int bgXPos, bgYPos;

	// bg text content
	std::wstring bgContent;
	
	// rows and columns of bgText characters required to fill window
	int charCols, charRows;

	// character set for bgText
	std::wstring bgCharSet;

	// refresh delay
	float refreshDelay;
	
	// refresh timer
	float refreshTimer;


	// title states

	// menu title text printer (first half)
	Text menuTitleText1;

	// menu title text printer (second half including ":")
	Text menuTitleText2;

	// menu title text content (first half)
	std::wstring titleStr1;

	// menu title text content (second half including ":")
	std::wstring titleStr2;

	// space between title text halves
	int titleSpacing;

	// menu title text offsets
	int title1XPos, title1YPos, title2XPos, title2YPos;

	// second title half highlight state
	bool title2Highlighted;

	// second title half highlight color
	COLOR_ARGB title2HighlightCol;

	// blink delay
	float titleBlinkDelay;

	// refresh timer
	float titleBlinkTimer;


	// menu states

	// menu normal text printer
	Text menuText;

	// menu options
	std::vector<MenuOption> options;

	// option spacing
	int optionSpacing;

	// option selected prefix
	std::wstring prefixStr;

	// selected menu option index
	int selectedMenuIndex;

	
	// working states

	// store window dimensions for easy access
	int windowW, windowH;


	// shared methods

	// initialize shared or common states
	void initializeSharedStates();


	// dynamic scene bg methods

	// initialize background states
	void initializeBackground();

	// update background text
	void updateBackground(float prevFrameTime);


	// scene title methods

	// initialize title states
	void initializeTitle();

	// update title states
	void updateTitle(float prevFrameTime);

	// draw title (should be called during sprite draw sequence)
	void renderTitle();


	// menu methods

	// initialize menu states
	void initializeMenu();

	// update menu selection states
	void updateSelectedStates(int indexModifier);

	// update menu states
	void updateMenu(float prevFrameTime);

	// draw menu (should be called during sprite draw sequence)
	void renderMenu();


	// transition methods

	// transition to scene with transition filter
	void transitionToScene(std::string sceneName);

	// update transition states
	void updateTransition(float prevFrameTime);

public:

	// default constructor
	MainMenuScene();

	// default destructor
	~MainMenuScene() {}


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
	virtual void renderBackground();

	// releases all memory reserved for graphics objects so that the bound
	// graphics device can be reset. called when the graphics device is lost.
	virtual void releaseAllGraphics();

	// recreates and restores all graphics objects. called after a lost
	// graphics device is restored.
	virtual void resetAllGraphics();
};

#endif // !_SCENE_MAINMENU_H