// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module:			Gameplay Programming
// Assignment1:		Cheats:Enabled
// Student Name:	Eugene Long - [Engine Designer]
// Student No.:		S10193060J
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// ===========================================================================
// InkdotEngine - Scene Interface Specification
// ===========================================================================
// * This is a InkdotEngine component, derived from the standard GPP engine
//   templates provided by Ms. Lee Ching Yun, modified by Eugene Long.
// ===========================================================================

#ifndef _INKDOT_SCENEINTERFACE_H
#define _INKDOT_SCENEINTERFACE_H

// import necessary headers
#include "../graphics/graphics.h"
#include "../input/input.h"


// class specification

class IScene
{
public:

	// abstract getters to be implemented by child classes

	// should return a pointer to the current graphics handler
	virtual Graphics* getGraphics() const = 0;

	// should return a pointer to the current input handler
	virtual Input* getInput() const = 0;

};

#endif // !_INKDOT_SCENEINTERFACE_H