// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module:			Gameplay Programming
// Assignment1:		Cheats:Enabled
// Student Name:	Eugene Long
// Student No.:		S10193060J
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// ===========================================================================
// InkdotEngine - Wall Entity Class Specification
// ===========================================================================

#ifndef _ENTITY_WALL_H
#define _ENTITY_WALL_H

// include necessary headers
#include "../../inkdot/include.h"


// class specification

class Wall : public InkdotEntity
{
private:
	COLLISION_LIST collisionList;
	// rect states

	float topLeftX;
	float topLeftY;
	float width;
	float height;

public:

	// default constructor
	Wall();

	// default destructor
	~Wall();


	// entity methods

	// initializes the entity - called on scene initialization
	virtual void initialize(
		float centerX, 
		float centerY, 
		float xEdge, 
		float yEdge
	);

	// renders entity sprites - called after background sprites are drawn
	virtual void renderSprites();

	// updates the entity - called each frame after scene update() ends
	virtual void onUpdateEnd(float prevFrameTime);
};

#endif // !_ENTITY_WALL_H