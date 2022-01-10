// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module:			Gameplay Programming
// Assignment1:		Cheats:Enabled
// Student Name:	Eugene Long - [Engine Designer]
// Student No.:		S10193060J
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// ===========================================================================
// InkdotEngine - Game Class Specification
// ===========================================================================
// * This is a InkdotEngine component, derived from the standard GPP engine
//   templates provided by Ms. Lee Ching Yun, modified by Eugene Long.
// ===========================================================================

#ifndef _INKDOT_GAME_H
#define _INKDOT_GAME_H

// import minimal windows headers
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// import necessary headers
#include "../graphics/graphics.h"
#include "../graphics/text.h"
#include "../input/input.h"
#include "../common.h"
#include "../error.h"
#include "sceneManager.h"
#include <string>

// import resources
#include "../resources/scenes/startup/startup.h"

// include mmsystem header and accompanying winmm lib
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")


// class specification

class Game
{
protected:

	// members

	// handle for game window
	HWND	hwnd;

	// specifies if the game is paused
	bool	paused;

	
	// handlers
	
	// pointer to graphics handler
	Graphics*		graphics;

	// pointer to input handler
	Input*			input;

	// pointer to scene manager
	SceneManager*	sceneManager;


	// game performance states

	// time elapsed to process the last frame (in seconds)
	float	frameTime;

	// time delay necessary to match minimum frame time (in seconds)
	DWORD	sleepTime;

	// average frames processed per second
	float	fps;

	// fps counter font
	Text	fpsText;

	// specifies if the fps counter is enabled
	bool	fpsEnabled;


	// working states

	// specifies when the frame timer last started (in ticks)
	LARGE_INTEGER	timeStart;

	// specifies when the frame timer last stopped (in ticks)
	LARGE_INTEGER	timeEnd;

	// contains the current performance timer frequency (ticks/sec) 
	LARGE_INTEGER	timerFreq;

	
	// misc

	// flag specifying if object has been initialized
	bool	initialized;

	// common result container
	bool	res;
	
public:

	// default constructor
	Game();

	// default destructor
	~Game();


	// methods

	// window message handler
	LRESULT wndProcHandler(
		HWND		hwnd,
		UINT		msg,
		WPARAM		wParam,
		LPARAM		lParam
	);

	// handle lost graphics device
	void handleLostGraphicsDevice();


	// extensible game methods

	/// <summary>
	/// Initializes the Game object instance. called only once on startup.
	/// </summary>
	/// <param name="hwnd">
	/// : Handle to the window that the game should be associated with
	/// </param>
	virtual void initialize(
		HWND	hwnd
	);

	// Runs each tick of the game - should be called within the main message
	// loop when not handling window events.
	virtual void run();

	// renders a single frame of the game. called every run cycle.
	virtual void render();

	// releases all memory reserved for graphics objects so that the bound
	// graphics device can be reset. called when the graphics device is lost.
	virtual void releaseAllGraphics();

	// recreates and restores all graphics objects. called after a lost
	// graphics device is restored.
	virtual void resetAllGraphics();

	// deallocate all manually allocated scene objects and reset object to the
	// pre-initialized state. will be called on instance destruction.
	virtual void deleteAll();

};

#endif // !_INKDOT_GAME_H