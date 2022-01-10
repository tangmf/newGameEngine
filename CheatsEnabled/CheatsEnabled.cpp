// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module:			Gameplay Programming
// Assignment1:		Cheats:Enabled
// Student Name:	Eugene Long - [Engine Designer]
// Student No.:		S10193060J
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// ===========================================================================
// InkdotEngine - WinMain
// ===========================================================================
// * This is a InkdotEngine component, derived from the standard GPP engine
// templates provided by Ms. Lee Ching Yun, modified by Eugene Long.
// ===========================================================================

// import minimal windows headers
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// import necessary headers
#include "CEGame.h"
#include <uxtheme.h>
#include <dwmapi.h>
#include <stdlib.h>

// include headers for memory leak detection
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

// forward function declarations
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int);
bool CreateMainWindow(HWND& hwnd, HINSTANCE hInstance, int nCmdShow);
LRESULT WINAPI WinProc(HWND, UINT, WPARAM, LPARAM);


// globals

// handle for current game window
HWND hwnd = NULL;

// pointer to current game
CEGame* game = NULL;


// function definitions

int WINAPI WinMain(
	_In_		HINSTANCE	hInstance,
	_In_opt_	HINSTANCE	hPrevInstance,
	_In_		LPSTR		lpCmdLine,
	_In_		int			nCmdShow
) {

	// check for memory leak on debug builds
	#if defined(DEBUG) | defined(_DEBUG)
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	#endif

	// initialize message loop variables
	MSG msg{};
	BOOL exit = FALSE;

	// construct game
	game = new CEGame();

	// create game window and ensure it exists
	if (!CreateMainWindow(hwnd, hInstance, nCmdShow)) return false;

	// run game
	try
	{

		// initialize game
		game->initialize(hwnd);

		// start main message loop
		while (!exit) {

			// handle new messages (non-blocking)
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				// exit loop on window quit event
				if (msg.message == WM_QUIT) exit = true;

				// translate and dispatch message to wndproc
				TranslateMessage(&msg);
				DispatchMessageW(&msg);

			}

			// handle game tick
			game->run();

		}

	}
	catch (const InkdotError& err)
	{
		// free game resources and destroy game window
		game->deleteAll();
		DestroyWindow(hwnd);

		// show error message
		MessageBoxA(NULL, err.getMessage(), "Error", MB_OK);

	}
	catch (...)
	{
		// free game resources and destroy game window
		game->deleteAll();
		DestroyWindow(hwnd);

		// show error message
		MessageBoxA(NULL, "Unknown error occured in game", "Error", MB_OK);
	}

	// delete game object before exit
	SAFE_DELETE(game);

	// return final message params on exit
	return msg.wParam;
}

bool CreateMainWindow(
	HWND& hwnd,
	HINSTANCE	hInstance,
	int			nCmdShow
) {
	// define variables
	DWORD		style;
	WNDCLASSEX	wcx;

	// initialize wcx
	ZeroMemory(&wcx, sizeof(wcx));

	// specify main window class params
	wcx.cbSize = sizeof(wcx);
	wcx.style = CS_HREDRAW | CS_VREDRAW;
	wcx.lpfnWndProc = WinProc;
	wcx.cbClsExtra = 0;
	wcx.cbWndExtra = 0;
	wcx.hInstance = hInstance;
	wcx.hIcon = NULL;
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcx.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcx.lpszMenuName = NULL;
	wcx.lpszClassName = GameConstants::CLASS_NAME.c_str();
	wcx.hIconSm = NULL;

	// register main window & guard against registration failure
	if (RegisterClassEx(&wcx) == 0) return false;

	// set style according to fullscreen/window mode
	if (WindowConstants::FULLSCREEN)
		style = WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP;
	else
		style = WS_OVERLAPPEDWINDOW;

	// create main window
	hwnd = CreateWindowExW(
		NULL,
		GameConstants::CLASS_NAME.c_str(),
		GameConstants::GAME_TITLE.c_str(),
		style,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		WindowConstants::GAME_WIDTH,
		WindowConstants::GAME_HEIGHT,
		(HWND)NULL,
		(HMENU)NULL,
		hInstance,
		(LPVOID)NULL
	);

	// ensure window was created successfully
	if (!hwnd) return false;

	// adjust window size to GAME_WIDTH x GAME_HEIGHT (if not fullscreen)
	if (!WindowConstants::FULLSCREEN)
	{
		// get client area of window
		RECT clientRect;
		GetClientRect(hwnd, &clientRect);

		// resize window
		MoveWindow(
			hwnd,
			32,
			32,
			WindowConstants::GAME_WIDTH +
			(WindowConstants::GAME_WIDTH - clientRect.right),
			WindowConstants::GAME_HEIGHT +
			(WindowConstants::GAME_HEIGHT - clientRect.bottom),
			TRUE
		);
	}

	// present the constructed window
	ShowWindow(hwnd, nCmdShow);
	return true;
}

LRESULT WINAPI WinProc(
	HWND	hwnd,
	UINT	msg,
	WPARAM	wParam,
	LPARAM	lParam
) {
	// process window message
	return (game->wndProcHandler(hwnd, msg, wParam, lParam));
}
