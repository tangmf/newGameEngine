// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module:			Gameplay Programming
// Assignment1:		Cheats:Enabled
// Student Name:	Eugene Long - [Engine Designer]
// Student No.:		S10193060J
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// ===========================================================================
// InkdotEngine - Common Values & Macros
// ===========================================================================
// * This is a InkdotEngine component, derived from the standard GPP engine
//   templates provided by Ms. Lee Ching Yun, modified by Eugene Long.
// ===========================================================================

#ifndef _INKDOT_COMMON_H
#define _INKDOT_COMMON_H

// import minimal windows headers
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// import necessary headers
#include <string>

// ===========================================================================
// common macros
// ===========================================================================

// pointer safety macros

// safely delete pointer referenced object
#define SAFE_DELETE(ptr) { if(ptr) { delete(ptr);	ptr = NULL; } }

// safely release pointer referenced object
#define SAFE_RELEASE(ptr) { if(ptr) { ptr->Release(); ptr = NULL; } }

// safely delete pointer referenced array
#define SAFE_DELETE_ARRAY(ptr) { if(ptr) { delete [](ptr); ptr = NULL; } }

// safely call onLostDevice
#define SAFE_ON_LOST_DEVICE(ptr) { if(ptr) { ptr->onLostDevice(); } }

// safely call onResetDevice
#define SAFE_ON_RESET_DEVICE(ptr) { if(ptr) { ptr->onResetDevice(); } }


// color macros

// define argb color data alias for the project
#define COLOR_ARGB DWORD

// define convenience method to construct color data using a, r, g, b values
#define SETCOLOR_ARGB(a, r, g, b) \
	(DWORD)				\
	((a&0xff) << 24 ) |	\
	((r&0xff) << 16 ) |	\
	((g&0xff) <<  8 ) |	\
	(b&0xff)


// ===========================================================================
// common constants
// ===========================================================================

// common color values
namespace CommonColors
{
	// ARGB channel values range from 0 - 255 inclusive.
	// A = Alpha channel (0 = transparent, 255 = opaque)
	// RGB = Red, Green, Blue channels respectively

	// colorwheel

	const COLOR_ARGB PURPLE		= SETCOLOR_ARGB(255, 128,   0, 128);
	const COLOR_ARGB MAROON		= SETCOLOR_ARGB(255, 128,   0,   0);
	const COLOR_ARGB RED		= SETCOLOR_ARGB(255, 255,   0,   0);
	const COLOR_ARGB MAGENTA	= SETCOLOR_ARGB(255, 255,   0, 255);
	const COLOR_ARGB OLIVE		= SETCOLOR_ARGB(255, 128, 128,   0);
	const COLOR_ARGB BROWN		= SETCOLOR_ARGB(255, 139,  69,  19);
	const COLOR_ARGB ORANGE		= SETCOLOR_ARGB(255, 255, 165,   0);
	const COLOR_ARGB YELLOW		= SETCOLOR_ARGB(255, 255, 255,   0);
	const COLOR_ARGB GREEN		= SETCOLOR_ARGB(255,   0, 128,   0);
	const COLOR_ARGB LIME		= SETCOLOR_ARGB(255,   0, 255,   0);
	const COLOR_ARGB TEAL		= SETCOLOR_ARGB(255,   0, 128, 128);
	const COLOR_ARGB CYAN		= SETCOLOR_ARGB(255,   0, 255, 255);
	const COLOR_ARGB NAVY		= SETCOLOR_ARGB(255,   0,   0, 128);
	const COLOR_ARGB BLUE		= SETCOLOR_ARGB(255,   0,   0, 255);

	// greyscale

	const COLOR_ARGB WHITE		= SETCOLOR_ARGB(255, 255, 255, 255);
	const COLOR_ARGB LTGRAY		= SETCOLOR_ARGB(255, 192, 192, 192);
	const COLOR_ARGB GRAY		= SETCOLOR_ARGB(255, 128, 128, 128);
	const COLOR_ARGB BLACK		= SETCOLOR_ARGB(255,   0,   0,   0);

	// opacity compopnents

	const COLOR_ARGB ALPHA0		= SETCOLOR_ARGB(  0, 255, 255, 255);
	const COLOR_ARGB ALPHA25	= SETCOLOR_ARGB( 64, 255, 255, 255);
	const COLOR_ARGB ALPHA50	= SETCOLOR_ARGB(128, 255, 255, 255);
	const COLOR_ARGB ALPHA75	= SETCOLOR_ARGB(192, 255, 255, 255);
	const COLOR_ARGB ALPHAMASK	= SETCOLOR_ARGB(255,   0,   0,   0);

	// use to specify drawing with colorfilter
	const COLOR_ARGB FILTER		= SETCOLOR_ARGB(  0,   0,   0,   0);

	// transparency key color
	const COLOR_ARGB TRANS_KEY	= SETCOLOR_ARGB(  0, 255,   0, 255);
}

// universal game-related constants
namespace GameConstants
{
	// title

	const std::wstring GAME_TITLE	= L"InkdotEngine";
	const std::wstring CLASS_NAME	= L"InkdotEngine_PrimaryGameWindow";

	// math

	const double PI = 3.14159265;

	// graphics

	const float	MAX_Z_DEPTH = 256;

	// framerate

	const float	TGT_FRAMERATE = 144.0f;
	const float MIN_FRAMERATE =  10.0f;
	const float MIN_FRAMETIME =   1.0f / TGT_FRAMERATE;
	const float MAX_FRAMETIME =   1.0f / MIN_FRAMERATE;
	
	// fps counter

	const LPCWSTR		FPS_FONT	= L"Terminal";
	const int			FPS_SIZE	= 12;
	const int			FPS_XPOS	= 16;
	const int			FPS_YPOS	= 16;
	const COLOR_ARGB	FPS_COLOR	= CommonColors::BLACK;
	const COLOR_ARGB	FPS_BG		= CommonColors::LIME;

	// debug override

	const bool			DEBUG_ON	= TRUE;
}

// physics/kinematics constants
namespace PhysicsConstants
{
	// collider types
	enum class ColliderType { NONE, CIRCLE, BOX };

	// universal gravitational constant
	const float GRAVITY = 6.67428e-11f;

	// scene downward gravitational force constant
	const float SCENE_GRAVITY = 100.0f;
}

// game window constants
namespace WindowConstants
{
	const bool			FULLSCREEN	= false;
	const COLOR_ARGB	BACK_COLOR	= CommonColors::LIME;
	const UINT			GAME_WIDTH	= 640;
	const UINT			GAME_HEIGHT	= 480;
	const RECT			BOUNDS		= {0, 0, GAME_WIDTH, GAME_HEIGHT};
}

// game scene name constants
namespace SceneNames
{
	// reserved namespace - no scene
	const std::string	NO_SCENE	= "NO_SCENE";
	const std::string	EXIT_GAME	= "EXIT_GAME";
	const std::string	STARTUP		= "STARTUP";
	const std::string	MAIN_MENU	= "MAIN_MENU";
	const std::string	LEVELCREATOR = "LEVEL_CREATOR";
	const std::string	SCENE1		= "TEST_SCENE";

	// initial game scene after startup scene (link your scene id here)
	const std::string	INITIAL_GAME_SCENE = "MAIN_MENU";
}

// game directory storage constants
namespace StorageConstants
{
	const std::string GAME_DIRECTORY	= "CheatsEnabled";
	const std::string LEVEL_FILE		= "level.txt";
}

// image filepath constants
namespace ImagePaths {}
//TODO: implement images 

// key-map constants
namespace KeyMappings {}
//TODO: implement keymaps

#endif // !_INKDOT_COMMON_H