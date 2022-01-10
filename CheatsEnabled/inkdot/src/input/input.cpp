// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module:			Gameplay Programming
// Assignment1:		Cheats:Enabled
// Student Name:	Eugene Long - [Engine Designer]
// Student No.:		S10193060J
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// ===========================================================================
// InkdotEngine - Input Handler Implementation
// ===========================================================================
// * This is a InkdotEngine component, derived from the standard GPP engine
//   templates provided by Ms. Lee Ching Yun, modified by Eugene Long.
// ===========================================================================

// include specification
#include "input.h"

// ===========================================================================
// default constructor
// ===========================================================================
Input::Input() :

	// keyboard
	keysDown		(),
	keysPressed		(),
	textIn			(""),
	charIn			(0),
	isNewline		(true),

	// mouse
	mouseCaptured	(false),
	rawInputDevices	(),
	mouseX			(0),
	mouseY			(0),
	mouseRawX		(0),
	mouseRawY		(0),
	mouseLBtn		(false),
	mouseMBtn		(false),
	mouseRBtn		(false),
	mouseX1Btn		(false),
	mouseX2Btn		(false),
	
	// controllers
	controllers		()

{
	// initialize key state arrays
	for (size_t i=0; i<InputNS::KEYS_ARRAY_LEN; i++) keysDown[i]	= false;
	for (size_t i=0; i<InputNS::KEYS_ARRAY_LEN; i++) keysPressed[i] = false;

	// initialize controller vibration
	for (int i = 0; i < InputNS::MAX_CONTROLLERS; i++) {
		controllers[i].vibrateTimeLeft = 0;
		controllers[i].vibrateTimeRight = 0;
	}
}

// ===========================================================================
// default destructor
// ===========================================================================
Input::~Input()
{
	// release mouse if it was bound
	if (mouseCaptured) ReleaseCapture();
}


// methods

// ===========================================================================
// initialization method
// ===========================================================================
void Input::initialize(
	HWND	hwnd,
	BOOL	captureMouse
) {
	try {

		// update internal states according to arguments
		mouseCaptured = captureMouse;

		// register high-definition mouse device
		rawInputDevices[0].usUsagePage	= HID_USAGE_PAGE_GENERIC;
		rawInputDevices[0].usUsage		= HID_USAGE_GENERIC_MOUSE;
		rawInputDevices[0].dwFlags		= RIDEV_INPUTSINK;
		rawInputDevices[0].hwndTarget	= hwnd;

		RegisterRawInputDevices(
			rawInputDevices,
			1,
			sizeof(rawInputDevices[0])
		);

		// capture mouse if enabled
		if (captureMouse) SetCapture(hwnd);

		// clear controllers' states (possible since arr uses contiguous mem)
		ZeroMemory(
			controllers,
			sizeof(ControllerState) * InputNS::MAX_CONTROLLERS
		);

		// check for connected controllers
		checkControllers();
	}
	catch (...)
	{
		throw(
			InkdotError(
				InkdotErrorType::FATAL_ERROR,
				"Error initializing input handler!"
			)
		);
	}
}

// ===========================================================================
// update key states for the specified virtual key during keyDown events
// ===========================================================================
void Input::handleKeyDown(
	UCHAR vkey
) {
	// ensure virtual key code is within key state array range
	if (vkey >= InputNS::KEYS_ARRAY_LEN) return;

	// update both keyDown and keyPressed to true
	keysDown	[vkey]	= true;
	keysPressed	[vkey]	= true;
}

// ===========================================================================
// update key states for the specified virtual key during keyUp events
// ===========================================================================
void Input::handleKeyUp(
	UCHAR vkey
) {
	// ensure virtual key code is within key state array range
	if (vkey >= InputNS::KEYS_ARRAY_LEN) return;

	// update keyDown to false (keyPressed is only cleared on clear())
	keysDown[vkey] = false;
}

// ===========================================================================
// saves the most recently entered char input to the textIn string
// ===========================================================================
void Input::handleKeyIn(
	UCHAR vkey
) {
	// handle start of newline
	if (isNewline) {

		// clear textIn and remove newline status
		textIn.clear();
		isNewline = false;
	}

	// handle backspace
	if ((char) vkey == '\b') {

		// if characters exist, erase last character entered
		if (textIn.length() > 0) textIn.erase(textIn.size() - 1);
	}

	// handle all other characters
	else {

		// add character to textIn
		textIn += vkey;

		// save last character entered to charIn
		charIn = vkey;
	}

	// handle return by starting newline
	if ((char)vkey == '\r') isNewline = true;
}

// ===========================================================================
// checks if the specified virtual key is down
// ===========================================================================
bool Input::isKeyDown(
	UCHAR vkey
) const
{
	// ensure vkey is within buffer range and return false on failure
	if (vkey >= InputNS::KEYS_ARRAY_LEN) return false;

	// if so, return status accordingly
	return keysDown[vkey];
}

// ===========================================================================
// checks if the specified virtual key was pressed in the most recent frame
// ===========================================================================
bool Input::wasKeyPressed(
	UCHAR vkey
) const
{
	// ensure vkey is within buffer range and return false on failure
	if (vkey >= InputNS::KEYS_ARRAY_LEN) return false;

	// if so, return status accordingly
	return keysPressed[vkey];
}

// ===========================================================================
// checks if any key was pressed in the most recent frame
// ===========================================================================
bool Input::anyKeyPressed() const
{
	// scan through pressed keys array for a pressed key
	for (size_t i = 0; i < InputNS::KEYS_ARRAY_LEN; i++)
		if (keysPressed[i] == true) return true;

	// else return false
	return false;
}

// ===========================================================================
// clear state data for the specified virtual key
// ===========================================================================
void Input::clearKeyPress(
	UCHAR	vkey
) {
	// ensure vkey is within buffer range and update states accordingly
	if (vkey < InputNS::KEYS_ARRAY_LEN) keysPressed[vkey] = false;
}

// ===========================================================================
// clear state buffers for the specified input types
// ===========================================================================
void Input::clear(
	UCHAR	inputBufferType
) {
	// clear keydown buffer if specified
	if (inputBufferType & InputBuffers::KEYS_DOWN) {
		for (size_t i = 0; i < InputNS::KEYS_ARRAY_LEN; i++)
			keysDown[i] = false;
	}

	// clear keypressed buffer if specified
	if (inputBufferType & InputBuffers::KEYS_PRESSED) {
		for (size_t i = 0; i < InputNS::KEYS_ARRAY_LEN; i++)
			keysPressed[i] = false;
	}

	// clear mouse state if specified
	if (inputBufferType & InputBuffers::MOUSE) {
		mouseX = 0;
		mouseY = 0;
		mouseRawX = 0;
		mouseRawY = 0;
	}

	// clear textIn input buffer if specified
	if (inputBufferType & InputBuffers::TEXT_IN) textIn.clear();
}

// ===========================================================================
// read the position of the mouse cursor into mouseX, mouseY
// ===========================================================================
void Input::mouseIn(
	LPARAM	lParam
) {
	mouseX = GET_X_LPARAM(lParam);
	mouseY = GET_Y_LPARAM(lParam);
}

// ===========================================================================
// read raw mouse movement data into mouseRawX, mouseRawY
// * provides compatibility for high definition mouse devices
// ===========================================================================
void Input::mouseRawIn(
	LPARAM	lParam
) {
	// initialise RAWINPUT struct and required data for processing
	RAWINPUT rawInput;
	UINT rawInputSize = sizeof(rawInput);
	ZeroMemory(&rawInput, rawInputSize);

	// retrieve raw input data to the RAWINPUT struct
	GetRawInputData(
		(HRAWINPUT)lParam,	// the handle to the RAWINPUT struct from lParam
		RID_INPUT,			// specifies raw input as the data to be retrieved
		&rawInput,			// pointer to the RAWINPUT struct
		&rawInputSize,
		sizeof(RAWINPUTHEADER)
	);

	// update raw mouse data states
	if (rawInput.header.dwSize == RIM_TYPEMOUSE) {
		mouseRawX = rawInput.data.mouse.lLastX;
		mouseRawY = rawInput.data.mouse.lLastY;
	}
}

// ===========================================================================
// check for and update the connection status for all controllers
// ===========================================================================
void Input::checkControllers()
{
	// initialise temp result container
	DWORD result;

	// iterate through all controllers
	for (DWORD i = 0; i < InputNS::MAX_CONTROLLERS; i++) {

		// attempt to retrieve state for controller i
		result = XInputGetState(i, &controllers[i].state);

		// update connection state according to result
		if (result == ERROR_SUCCESS)	controllers[i].connected = true;
		else							controllers[i].connected = false;
	}
}

// ===========================================================================
// read and update states for all connected controllers
// ===========================================================================
void Input::updateControllers()
{
	// initialise temp result container
	DWORD result;

	// iterate through all controllers
	for (DWORD i = 0; i < InputNS::MAX_CONTROLLERS; i++) {

		// ensure controller is connected
		if (!controllers[i].connected) return;

		// read and save result to state
		result = XInputGetState(i, &controllers[i].state);

		// handle controller disconnected
		if (result == ERROR_DEVICE_NOT_CONNECTED)
			controllers[i].connected = false;
	}
}

// ===========================================================================
// vibrates connected controllers for the specified amount of time
// ===========================================================================
void Input::vibrateControllers(
	float	frameTime
) {
	// iterate through all controllers
	for (int i = 0; i < InputNS::MAX_CONTROLLERS; i++) {

		// ensure controller is connected
		if (!controllers[i].connected) return;

		// decrement remaining frame time
		controllers[i].vibrateTimeLeft -= frameTime;
		controllers[i].vibrateTimeRight -= frameTime;

		// if remaining time is below zero, reset vibration states
		if (controllers[i].vibrateTimeLeft < 0) gamepadVibrateLeft(i, 0, 0);
		if (controllers[i].vibrateTimeRight < 0) gamepadVibrateRight(i, 0, 0);

		// set controller states accordingly
		XInputSetState(i, &controllers[i].vibration);
	}
}
