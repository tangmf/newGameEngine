// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module:			Gameplay Programming
// Assignment1:		Cheats:Enabled
// Student Name:	Eugene Long - [Engine Designer]
// Student No.:		S10193060J
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// ===========================================================================
// InkdotEngine - Entity Component Class Specification
// ===========================================================================
// * This is a InkdotEngine component, derived from the standard GPP engine
//   templates provided by Ms. Lee Ching Yun, modified by Eugene Long.
// ===========================================================================

#ifndef _INKDOT_ENTITYCOMPONENT_H
#define _INKDOT_ENTITYCOMPONENT_H

// include necessary headers
#include "collider.h"

// class specification

class EntityComponent
{
protected:

	// the position of the component's center relative to the entity's center
	VECTOR2 relativeCenter;

public:

	// constructor
	EntityComponent(float relX, float relY): relativeCenter(relX, relY) {}

	// destructor
	~EntityComponent() {}


	// getters

	// return the relative pos. of the component center to the entity center
	const VECTOR2* getRelativeCenter() const
	{ return &relativeCenter; }


	// setters

	// sets the relative pos. of the component center to the entity center
	void setRelativeCenter(VECTOR2 pos)
	{ relativeCenter = pos; }


	// abstract setters

	// should set the position of the component to pos
	virtual void setComponentPosition(VECTOR2 pos) = 0;
};

#endif // !_INKDOT_ENTITYCOMPONENT_H