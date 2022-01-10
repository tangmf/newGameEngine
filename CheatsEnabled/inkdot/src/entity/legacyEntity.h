// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module:			Gameplay Programming
// Assignment1:		Cheats:Enabled
// Student Name:	Eugene Long - [Engine Designer]
// Student No.:		S10193060J
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// ===========================================================================
// InkdotEngine - Legacy Entity Class Specification
// ===========================================================================
// * This is a InkdotEngine component, derived from the standard GPP engine
//   templates provided by Ms. Lee Ching Yun, modified by Eugene Long.
// ===========================================================================

#ifndef _INKDOT_LEGACYENTITY_H
#define _INKDOT_LEGACYENTITY_H

// include necessary headers
#include "entity.h"
#include "../graphics/image.h"


// related constructs

class EntityImage : public Image, public EntityComponent
{
public:

	// default constructor
	EntityImage() :

		// base collider constructor
		Image(),

		// base entity component constructor
		EntityComponent(0, 0)
	{}

	// default destructor
	~EntityImage() {}

	
	// methods

	// recalculate relative position such that image is centered on entity
	void recenterImage();


	// setter overrides

	// recalculate entity image relative position on scale update
	virtual void setScale(float s);


	// component setters

	// sets the position of the component to pos
	virtual void setComponentPosition(VECTOR2 pos)
	{ setX(pos.x); setY(pos.y); }
};


// class specification

class InkdotLegacyEntity : public InkdotEntity
{
protected:

	// legacy entity states

	// entity sprite
	EntityImage sprite;

public:

	// default constructor
	InkdotLegacyEntity();

	// default destructor
	~InkdotLegacyEntity();


	// extendable entity methods

	// initializes the entity - called on scene initialization
	virtual void initialize(
		Graphics*		g,
		int				width,
		int				height,
		int				nCols,
		TextureManager*	tm,
		float			initialX	= 0,
		float			initialY	= 0,
		float			_mass		= 1.0f,
		float			_gravity	= 1.0f
	);

	// updates the entity - called each frame after scene update() ends
	virtual void onUpdateEnd(float prevFrameTime);

	// renders entity sprites - called after background sprites are drawn
	virtual void renderSprites();

	// releases all memory reserved for graphics objects so that the bound
	// graphics device can be reset - called when the graphics device is lost
	virtual void releaseAllGraphics() {}

	// recreates and restores all graphics objects - called when the lost
	// graphics device is restored
	virtual void resetAllGraphics() {}


	// getters

	// return pointer to entity sprite
	Image* getSprite()
	{ return &sprite; }


	// extendable setters

	// set position of entity center using vector
	virtual void setCenter(VECTOR2 pos);

	// set entity rotation (in radians)
	virtual void setRotation(float r);

	// set entity scale
	virtual void setScale(float s);

	// set entity visible status
	virtual void setActive(bool e);

	// set entity visible state
	virtual void setVisible(bool e);

};

#endif // !_INKDOT_LEGACYENTITY_H