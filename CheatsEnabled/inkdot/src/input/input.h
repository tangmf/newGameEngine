// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module:			Gameplay Programming
// Assignment1:		Cheats:Enabled
// Student Name:	Eugene Long - [Engine Designer]
// Student No.:		S10193060J
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// ===========================================================================
// InkdotEngine - Input Handler Specification
// ===========================================================================
// * This is a InkdotEngine component, derived from the standard GPP engine
//   templates provided by Ms. Lee Ching Yun, modified by Eugene Long.
// ===========================================================================

#ifndef _INKDOT_INPUT_H
#define _INKDOT_INPUT_H

// import minimal windows headers
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// import necessary headers
#include <windowsx.h>
#include <string>
#include "../error.h"
#include "../common.h"

// include xinput header and libs
#include <Xinput.h>
#pragma comment(lib, "xinput.lib")

// define macros for high-definition mouse devices
#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC	((USHORT) 0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE ((USHORT) 0x02)
#endif


// related constructs

namespace InputBuffers
{
	// bit flags for clear()

	const UCHAR KEYS_DOWN		= 0b0001;
	const UCHAR KEYS_PRESSED	= 0b0010;
	const UCHAR MOUSE			= 0b0100;
	const UCHAR TEXT_IN			= 0b1000;
	const UCHAR ALL_INPUT		= 0b1111;
}

namespace InputConstants
{
	// key array size
	const int KEYS_ARRAY_LEN = 256;

	// thumbstick deadzone - default 20%
	const DWORD GAMEPAD_THUMBSTICK_DEADZONE = (DWORD)(0.20f * 0x7FFF);

	// trigger deadzone - default 30/255
	const DWORD GAMEPAD_TRIGGER_DEADZONE = 30;

	// maximum controllers allowed
	const DWORD	MAX_CONTROLLERS = 4;

	// gamepad button bit flags (according to state.Gamepad.wButtons)

	const DWORD GAMEPAD_DPAD_UP			= 0x0001;
	const DWORD GAMEPAD_DPAD_DOWN		= 0x0002;
	const DWORD GAMEPAD_DPAD_LEFT		= 0x0004;
	const DWORD GAMEPAD_DPAD_RIGHT		= 0x0008;
	const DWORD GAMEPAD_START_BUTTON	= 0x0010;
	const DWORD GAMEPAD_BACK_BUTTON		= 0x0020;
	const DWORD GAMEPAD_LEFT_THUMB		= 0x0040;
	const DWORD GAMEPAD_RIGHT_THUMB		= 0x0080;
	const DWORD GAMEPAD_LEFT_SHOULDER	= 0x0100;
	const DWORD GAMEPAD_RIGHT_SHOULDER	= 0x0200;
	const DWORD GAMEPAD_A				= 0x1000;
	const DWORD GAMEPAD_B				= 0x2000;
	const DWORD GAMEPAD_X				= 0x4000;
	const DWORD GAMEPAD_Y				= 0x8000;
}

// add shortform alias for convenience
#define InputNS InputConstants

struct ControllerState
{
	XINPUT_STATE		state;
	XINPUT_VIBRATION	vibration;
	float				vibrateTimeLeft;
	float				vibrateTimeRight;
	bool				connected;
};


// class specification

class Input
{
private:

	// keyboard states

	// indicates if the i-th key is down
	bool keysDown[InputConstants::KEYS_ARRAY_LEN];

	// indicates if the i-th key was pressed in the most recent frame
	bool keysPressed[InputConstants::KEYS_ARRAY_LEN];

	// stores user-entered text
	std::string textIn;

	// stores the last character entered
	char charIn;

	// indicates if it is currently the start of a new line
	bool isNewline;

	
	// mouse states

	// mouse screen coordinates
	int mouseX, mouseY;

	// high-definition mouse input states
	int mouseRawX, mouseRawY;

	// high-definition mouse handler
	RAWINPUTDEVICE rawInputDevices[1];

	// indicates if the mouse should be captured
	bool mouseCaptured;

	// mouse button states
	bool mouseLBtn, mouseMBtn, mouseRBtn, mouseX1Btn, mouseX2Btn;


	// controller states
	ControllerState controllers[InputConstants::MAX_CONTROLLERS];


	// methods

	// limits n to MAX_CONTROLLERS - 1 if n >= MAX_CONTROLLERS
	int getValidControllerIndex(int n)
	{
		return n >= InputConstants::MAX_CONTROLLERS
			? InputConstants::MAX_CONTROLLERS - 1
			: n;
	}

public:

	// default constructor
	Input();

	// default destructor
	~Input();

	/// <summary>
	/// Initializes the Input handler object instance.
	/// </summary>
	/// <param name="hwnd">
	/// : The handle of the window to bind this handler to.
	/// </param>
	/// <param name="captureMouse">
	/// : Specifies whether the mouse should be captured by the instance.
	/// </param>
	void initialize(
		HWND	hwnd,
		BOOL	captureMouse
	);


	// keyboard methods

	// updates the relevant key-states for the virtual key on keydown
	void handleKeyDown(UCHAR vkey);

	// updates the relevant key-states for the virtual key on keyup
	void handleKeyUp(UCHAR vkey);

	// saves the character entered by the user into the textIn string
	void handleKeyIn(UCHAR vkey);

	// checks if the specified key is currently down
	bool isKeyDown(UCHAR vkey) const;

	// checks if the specified key was pressed in the most recent frame - note
	// that key presses are erased at the end of each frame
	bool wasKeyPressed(UCHAR vkey) const;

	// checks if any keys were pressed in the most recent frame
	bool anyKeyPressed() const;

	// clears the specified key press
	void clearKeyPress(UCHAR vkey);

	/// <summary>
	/// Clears the input buffers for the specified input type(s).
	/// </summary>
	/// <param name="inputType">
	/// : Any combination of KEYS_DOWN, KEYS_PRESSED, MOUSE, TEXT_IN or 
	///   ALL_INPUT. Use the OR "|" operator to combine types.
	/// </param>
	void clear(UCHAR inputType);

	// clears the text input buffer
	void clearTextIn() { textIn.clear(); }

	// clears all input buffers
	void clearAll() { clear(InputBuffers::ALL_INPUT); }

	// retrieves the text input buffer
	std::string getTextIn() { return textIn; }

	// retrieves the most recently entered character
	char getCharIn() { return charIn; }


	// mice methods

	// reads the mouse position from lParam into mouseX, mouseY
	void mouseIn(LPARAM);

	// reads the raw mouse input from lParam into mouseRawX, mouseRawY - this
	// routine provides compatibility for high-definition mouse devices
	void mouseRawIn(LPARAM);

	// saves the state of the left mouse button
	void setMouseLBtn(bool state) { mouseLBtn = state; }

	// saves the state of the middle mouse button
	void setMouseMBtn(bool state) { mouseMBtn = state; }

	// saves the state of the right mouse button
	void setMouseRBtn(bool state) { mouseRBtn = state; }

	// saves the state of the extra mouse buttons
	void setMouseXBtn(WPARAM wParam)
	{
		mouseX1Btn = (wParam & MK_XBUTTON1) ? true : false;
		mouseX2Btn = (wParam & MK_XBUTTON2) ? true : false;
	}

	// returns the current x-position of the mouse
	int getMouseX() const { return mouseX; }

	// returns the current y-position of the mouse
	int getMouseY() const { return mouseY; }

	// returns the raw x-axis movement of the mouse. (<0 = left, >0 = right)
	// compatible with high-definition mouse devices.
	int getMouseRawX() const { return mouseRawX; }

	// returns the raw y-axis movement of the mouse. (<0 = down, >0 = up)
	// compatible with high-definition mouse devices.
	int getMouseRawY() const { return mouseRawY; }

	// returns the state of the left mouse button
	bool getMouseLBtn() const { return mouseLBtn; }

	// returns the state of the middle mouse button
	bool getMouseMBtn() const { return mouseMBtn; }

	// returns the state of the right mouse button
	bool getMouseRBtn() const { return mouseRBtn; }

	// returns the state of extra mouse button 1 (m4)
	bool getMouseX1Btn() const { return mouseX1Btn; }

	// returns the state of extra mouse button 2 (m5)
	bool getMouseX2Btn() const { return mouseX2Btn; }


	// controller methods

	// updates the connection status for all game controllers
	void checkControllers();

	// updates the saved states of all connected controllers
	void updateControllers();

	// returns a reference to the state for the specified game controller
	const ControllerState* getGamepadState(UINT n)
	{
		return &controllers[getValidControllerIndex(n)];
	}

	// returns the button states for the specified game controller
	const WORD getGamepadBtns(UINT n) {
		return controllers[getValidControllerIndex(n)].state.Gamepad.wButtons;
	}

	// returns the d-pad up button state for the specified controller
	bool getGamepadDPadUp(UINT n) { 
		return (getGamepadBtns(n) & InputNS::GAMEPAD_DPAD_UP) != 0;
	}

	// returns the d-pad down button state for the specified controller
	bool getGamepadDPadDown(UINT n) {
		return (getGamepadBtns(n) & InputNS::GAMEPAD_DPAD_DOWN) != 0;
	}

	// returns the d-pad left button state for the specified controller
	bool getGamepadDPadLeft(UINT n) {
		return (getGamepadBtns(n) & InputNS::GAMEPAD_DPAD_LEFT) != 0;
	}

	// returns the d-pad right button state for the specified controller
	bool getGamepadDPadRight(UINT n) {
		return (getGamepadBtns(n) & InputNS::GAMEPAD_DPAD_RIGHT) != 0;
	}

	// returns the start button state for the specified controller
	bool getGamepadStart(UINT n) {
		return (getGamepadBtns(n) & InputNS::GAMEPAD_START_BUTTON) != 0;
	}

	// returns the back button state for the specified controller
	bool getGamepadBack(UINT n) {
		return (getGamepadBtns(n) & InputNS::GAMEPAD_BACK_BUTTON) != 0;
	}

	// returns the left thumb button state for the specified controller
	bool getGamepadLeftThumb(UINT n) {
		return (getGamepadBtns(n) & InputNS::GAMEPAD_LEFT_THUMB) != 0;
	}

	// returns the right thumb button state for the specified controller
	bool getGamepadRightThumb(UINT n) {
		return (getGamepadBtns(n) & InputNS::GAMEPAD_RIGHT_THUMB) != 0;
	}

	// returns the left shoulder button state for the specified controller
	bool getGamepadLeftShoulder(UINT n) {
		return (getGamepadBtns(n) & InputNS::GAMEPAD_LEFT_SHOULDER) != 0;
	}

	// returns the right shoulder button state for the specified controller
	bool getGamepadRightShoulder(UINT n) {
		return (getGamepadBtns(n) & InputNS::GAMEPAD_RIGHT_SHOULDER) != 0;
	}

	// returns the "A" button state for the specified controller
	bool getGamepadA(UINT n) {
		return (getGamepadBtns(n) & InputNS::GAMEPAD_A) != 0;
	}

	// returns the "B" button state for the specified controller
	bool getGamepadB(UINT n) {
		return (getGamepadBtns(n) & InputNS::GAMEPAD_B) != 0;
	}

	// returns the "X" button state for the specified controller
	bool getGamepadX(UINT n) {
		return (getGamepadBtns(n) & InputNS::GAMEPAD_X) != 0;
	}

	// returns the "Y" button state for the specified controller
	bool getGamepadY(UINT n) {
		return (getGamepadBtns(n) & InputNS::GAMEPAD_Y) != 0;
	}

	// returns the left trigger state for the specified controller
	BYTE getGamepadLeftTrigger(UINT n) {
		return getGamepadState(n)->state.Gamepad.bLeftTrigger;
	}

	// returns the right trigger state for the specified controller
	BYTE getGamepadRightTrigger(UINT n) {
		return getGamepadState(n)->state.Gamepad.bRightTrigger;
	}

	// returns the left thumbstick's x-value for the specified controller
	SHORT getGamepadThumbLX(UINT n) {
		return getGamepadState(n)->state.Gamepad.sThumbLX;
	}

	// returns the left thumbstick' y-value for the specified controller
	SHORT getGamepadThumbLY(UINT n) {
		return getGamepadState(n)->state.Gamepad.sThumbLY;
	}

	// returns the right thumbstick's x-value for the specified controller
	SHORT getGamepadThumbRX(UINT n) {
		return getGamepadState(n)->state.Gamepad.sThumbRX;
	}

	// returns the right thumbstick's y-value for the specified controller
	SHORT getGamepadThumbRY(UINT n) {
		return getGamepadState(n)->state.Gamepad.sThumbRY;
	}

	/// <summary>
	/// Vibrates the specified game controller's left motor. (low freq.)
	/// </summary>
	/// <param name="n">
	/// : The index of the game controller
	/// </param>
	/// <param name="speed">
	/// : The vibration frequency. (0 = off, 65536 = maximum vibration)
	/// </param>
	/// <param name="sec">
	/// : The amount of time to vibrate for (in seconds)
	/// </param>
	void gamepadVibrateLeft(UINT n, WORD speed, float sec)
	{
		n = getValidControllerIndex(n);
		controllers[n].vibration.wLeftMotorSpeed = speed;
		controllers[n].vibrateTimeLeft = sec;
	}

	/// <summary>
	/// Vibrates the specified game controller's right motor. (high freq.)
	/// </summary>
	/// <param name="n">
	/// : The index of the game controller
	/// </param>
	/// <param name="speed">
	/// : The vibration frequency. (0 = off, 65536 = maximum vibration)
	/// </param>
	/// <param name="sec">
	/// : The amount of time to vibrate for (in seconds)
	/// </param>
	void gamepadVibrateRight(UINT n, WORD speed, float sec)
	{
		n = getValidControllerIndex(n);
		controllers[n].vibration.wRightMotorSpeed = speed;
		controllers[n].vibrateTimeRight = sec;
	}

	// vibrates the connected controllers for the specified time
	void vibrateControllers(float frameTime);

};


#endif // !_INKDOT_INPUT_H