// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module:			Gameplay Programming
// Assignment1:		Cheats:Enabled
// Student Name:	Eugene Long - [Engine Designer]
// Student No.:		S10193060J
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// ===========================================================================
// InkdotEngine - Game Class Implementation
// ===========================================================================
// * This is a InkdotEngine component, derived from the standard GPP engine
//   templates provided by Ms. Lee Ching Yun, modified by Eugene Long.
// ===========================================================================

// include specification
#include "game.h"

// ===========================================================================
// default constructor
// ===========================================================================
Game::Game():

	// members
	hwnd	(),
	paused	(FALSE),

	// handlers
	graphics		(NULL),
	input			(NULL),
	sceneManager	(NULL),

	// game performance
	frameTime	(0),
	sleepTime	(0),
	fps			(0),
	fpsText		(),
	fpsEnabled	(TRUE),

	// working
	timeStart	(),
	timeEnd		(),
	timerFreq	(),

	// misc
	initialized	(FALSE),
	res			(FALSE)
{
	// allocate handlers
	graphics		= new Graphics();
	input			= new Input();
	sceneManager	= new SceneManager();
}

// ===========================================================================
// default destructor
// ===========================================================================
Game::~Game()
{
	// deallocate all manually allocated objects
	deleteAll();
}


// methods

// ===========================================================================
// window message handler
// ===========================================================================
LRESULT Game::wndProcHandler(
	HWND		hwnd,
	UINT		msg,
	WPARAM		wParam,
	LPARAM		lParam
) {
	// return default window process results if not initialized
	if (!initialized) return DefWindowProc(hwnd, msg, wParam, lParam);

	// else process message using custom logic
	switch (msg)
	{

	// on window destruction, tell windows to kill this program
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	// handle inputs

	// handle key down
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	{
		input->handleKeyDown(wParam);
		return 0;
	}

	// handle key up
	case WM_KEYUP:
	case WM_SYSKEYUP:
	{
		input->handleKeyUp(wParam);
		return 0;
	}

	// handle character entered
	case WM_CHAR:
	{
		input->handleKeyIn(wParam);
		return 0;
	}

	// handle mouse movement
	case WM_MOUSEMOVE:
	{
		input->mouseIn(lParam);
		return 0;
	}

	// handle raw mouse data input
	case WM_INPUT:
	{
		input->mouseRawIn(lParam);
		return 0;
	}

	// handle mouse left button down, update mouse position
	case WM_LBUTTONDOWN:
	{
		input->setMouseLBtn(true);
		input->mouseIn(lParam);
		return 0;
	}

	// handle mouse left button up, update mouse position
	case WM_LBUTTONUP:
	{
		input->setMouseLBtn(false);
		input->mouseIn(lParam);
		return 0;
	}

	// handle mouse middle button down, update mouse position
	case WM_MBUTTONDOWN:
	{
		input->setMouseMBtn(true);
		input->mouseIn(lParam);
		return 0;
	}

	// handle mouse middle button up, update mouse position
	case WM_MBUTTONUP:
	{
		input->setMouseMBtn(false);
		input->mouseIn(lParam);
		return 0;
	}

	// handle mouse right button down, update mouse position
	case WM_RBUTTONDOWN:
	{
		input->setMouseRBtn(true);
		input->mouseIn(lParam);
		return 0;
	}

	// handle mouse right button up, update mouse position
	case WM_RBUTTONUP:
	{
		input->setMouseRBtn(false);
		input->mouseIn(lParam);
		return 0;
	}

	// handle mouse x button down/up, update mouse position
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	{
		input->setMouseXBtn(wParam);
		input->mouseIn(lParam);
		return 0;
	}

	// check for controller insert
	case WM_DEVICECHANGE:
	{
		input->checkControllers();
		return 0;
	}

	} // end switch-case

	// else, default to defwindowproc
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

// ===========================================================================
// handle lost graphics device
// ===========================================================================
void Game::handleLostGraphicsDevice()
{
	// check if device is lost
	HRESULT hr = graphics->getDeviceState();

	// if graphics device is not in an invalid state, exit early
	if (!FAILED(hr)) return;

	// ensure device is available for reset.
	// if not, wait a while and try again next run cycle
	if (hr == D3DERR_DEVICELOST) {
		Sleep(100);
		return;
	}

	// if device is available for reset, reset device
	else if (hr == D3DERR_DEVICENOTRESET) {

		// release all graphics objects to allow for reset
		releaseAllGraphics();

		// attempt reset
		hr = graphics->resetDevice();

		// handle reset failed
		if (FAILED(hr)) return;

		// if successful, proceed to restore all graphics objects
		resetAllGraphics();
	}

	// other device error
	else return;
}


// extensible game methods

// ===========================================================================
// initializes the Game object instance. called only once on startup
// ===========================================================================
void Game::initialize(
	HWND	hwnd
) {
	// store argument info into instance state
	this->hwnd = hwnd;


	// initialize handlers

	// initialize graphics handler
	graphics->initialize(
		hwnd,
		WindowConstants::GAME_WIDTH,
		WindowConstants::GAME_HEIGHT,
		WindowConstants::FULLSCREEN
	);

	// initialize input handler - disable mouse capture by default
	input->initialize(hwnd, false);

	// initialize scene manager
	sceneManager->initialize(graphics, input);


	// initialize performance counter

	// set up high-resolution (<1 microsecond) timer
	res = QueryPerformanceFrequency(&timerFreq);
	if (res == false)
		throw(
			InkdotError(
				InkdotErrorType::FATAL_ERROR,
				"Error initializing high-resolution timer!"
			)
		);

	// initialize starting time
	QueryPerformanceCounter(&timeStart);


	// initialize fps display

	// initialize fps counter text font
	res = fpsText.initialize(
		graphics,
		GameConstants::FPS_SIZE,
		FALSE,
		FALSE,
		GameConstants::FPS_FONT
	);
	if (res == false)
		throw(
			InkdotError(
				InkdotErrorType::FATAL_ERROR,
				"Error initializing fps counter text font!"
			)
		);

	// set fps counter color
	fpsText.setTextColor(GameConstants::FPS_COLOR);
	fpsText.setHighlight(GameConstants::FPS_BG);


	// initialize engine start screen sequence

	// initialize start screen scene
	StartupScene* startup = new StartupScene();

	// register startup scene
	sceneManager->registerScene(startup, SceneNames::STARTUP);

	// transition to startup scene
	sceneManager->transitionToScene(SceneNames::STARTUP);


	// update status
	initialized = true;
}

// ===========================================================================
// runs each tick of the game - should be called within the main message loop
// when not handling window events.
// ===========================================================================
void Game::run()
{
	// ensure game has been initialized
	if (!initialized) return;


	// performance counter calculations

	// get ticks since last frame
	QueryPerformanceCounter(&timeEnd);

	// calculate elapsed frame time in seconds
	frameTime = 
		(float) (timeEnd.QuadPart - timeStart.QuadPart) /
		(float) timerFreq.QuadPart;

	// compensate if frameTime is faster than the minimum frametime to reach
	// target framerate by sleeping (power saving code, requires winmm.lib)
	if (frameTime < GameConstants::MIN_FRAMETIME)
	{
		// calculate additional sleep time required for minimum frame time (s)
		sleepTime = 
			(DWORD) ((GameConstants::MIN_FRAMETIME - frameTime) * 1000);

		// request 1ms resolution for windows timer
		timeBeginPeriod(1);

		// release cpu for sleepTime
		Sleep(sleepTime);

		// end 1ms timer resolution
		timeEndPeriod(1);

		// skip to next run cycle
		return;
	}

	// calculate fps
	if (frameTime > 0.0) fps = (fps * 0.99f) + (0.01f / frameTime);

	// limit maximum frametime if framerate is very slow
	if (frameTime > GameConstants::MAX_FRAMETIME)
		frameTime = GameConstants::MAX_FRAMETIME;

	// update time states (starts timer for this run cycle)
	timeStart = timeEnd;


	// run game logic

	// ensure that the game is running (not paused)
	if (!paused) {

		// run update(), ai() and collision() handlers for current scene
		sceneManager->runCurrentScene(frameTime);

		// handle controller vibration
		input->vibrateControllers(frameTime);
	}

	// draw all game items after processing is complete for current scene
	render();

	// update controller states
	input->updateControllers();

	// clear key-press input buffers at end of run cycle
	input->clear(InputBuffers::KEYS_PRESSED);
}

// ===========================================================================
// renders a single frame of the game. called every run cycle.
// ===========================================================================
void Game::render()
{
	// attempt to start scene drawing sequence
	if (SUCCEEDED(graphics->beginSceneDraw()))
	{
		// render all graphics objects for the current scene to backbuffer
		sceneManager->renderCurrentScene();

		// render the fps counter on top of scene (if enabled)
		if (fpsEnabled) {

			// convert and format fps for display
			std::wstring formattedFps = 
				L"FPS [" + std::to_wstring((int)fps) + L"]";

			// draw fps
			graphics->beginSpriteDraw();
			fpsText.print(
				formattedFps,
				GameConstants::FPS_XPOS,
				GameConstants::FPS_YPOS,
				0.0f
			);
			graphics->endSpriteDraw();
		}

		// stop rendering once rendering is complete
		graphics->endSceneDraw();
	}

	// check if graphics device is lost and attempt to recover the device
	handleLostGraphicsDevice();

	// finally, display the back buffer on the screen
	graphics->showBackBuffer();
}

// ===========================================================================
// releases all memory reserved for graphics objects so that the bound
// graphics device can be reset. called when the graphics device is lost.
// ===========================================================================
void Game::releaseAllGraphics()
{
	// release all graphics objects from memory for current scene
	sceneManager->releaseGraphicsForCurrentScene();
}

// ===========================================================================
// recreates and restores all graphics objects. called after a lost graphics
// device is restored.
// ===========================================================================
void Game::resetAllGraphics()
{
	// reset and restore all graphics objects for current scene
	sceneManager->resetGraphicsForCurrentScene();
}

// ===========================================================================
// deallocate all manually allocated scene objects and reset object to the
// pre-initialized state. will be called on instance destruction.
// ===========================================================================
void Game::deleteAll()
{
	// deallocate all graphic objects
	releaseAllGraphics();

	// delete handlers
	SAFE_DELETE(graphics);
	SAFE_DELETE(input);

	// update states
	initialized = false;
}
