// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module:			Gameplay Programming
// Assignment1:		Cheats:Enabled
// Student Name:	Eugene Long - [Engine Designer]
// Student No.:		S10193060J
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// ===========================================================================
// InkdotEngine - Legacy Entity Class Implementation
// ===========================================================================
// * This is a InkdotEngine component, derived from the standard GPP engine
//   templates provided by Ms. Lee Ching Yun, modified by Eugene Long.
// ===========================================================================

// include specification
#include "legacyEntity.h"


// related constructs

// ===========================================================================
// recalculate relative position such that image is centered on entity
// ===========================================================================
void EntityImage::recenterImage()
{
	// let top left of image be A
	// let bottom right of scaled image be B
	// let the center of the scaled image be C, which lies on the midpoint
	// of the line connecting A and B, AB

	// find CA: CA = -AC = 1/2BA = -1/2AB
	// ABx = (width * scale), ABy = (height * scale)
	
	// .: CA = width * scale * -0.5, height * scale * -0.5
	setRelativeCenter(VECTOR2(
		getWidth() * getScale() * -0.5f,
		getHeight() * getScale() * -0.5f
	));
}

// ===========================================================================
// recalculate entity image relative position on scale update
// ===========================================================================
void EntityImage::setScale(
	float	s
) {
	// get scale ratio
	float scaleRatio = s / getScale();

	// apply scale ratio to current relative position
	relativeCenter *= scaleRatio;

	// call base scale update function
	Image::setScale(s);
}


// class methods

// ===========================================================================
// default constructor
// ===========================================================================
InkdotLegacyEntity::InkdotLegacyEntity() :

	// base entity constructor
	InkdotEntity(),

	// entity sprite
	sprite	()
{}

// ===========================================================================
// default destructor
// ===========================================================================
InkdotLegacyEntity::~InkdotLegacyEntity() {}


// extendable entity methods

// ===========================================================================
// initializes the entity - called on scene initialization
// ===========================================================================
void InkdotLegacyEntity::initialize(
	Graphics*		g,
	int				width,
	int				height,
	int				nCols,
	TextureManager*	tm,
	float			initialX,
	float			initialY,
	float			_mass,
	float			_gravity
) {
	// store argument info into instance state
	mass	= _mass;
	gravity = _gravity;

	// initialize sprite
	if (!sprite.initialize(
		g,
		width,
		height,
		nCols,
		tm
	)) throw(
		InkdotError(
			InkdotErrorType::FATAL_ERROR,
			"Error inititalizing texture for entity!"
		)
		);

	// recenter image
	sprite.recenterImage();

	// get edges for sprite collider
	long xEdge = width / 2;
	long yEdge = height / 2;

	// add sprite collider
	addCollider(
		"sprite",
		0,
		0,
		{ -xEdge, -yEdge, xEdge, yEdge }
	);

	// set initial entity position
	setCenter(VECTOR2(initialX, initialY));
}

// ===========================================================================
// updates the entity - called each frame after scene update() ends
// ===========================================================================
void InkdotLegacyEntity::onUpdateEnd(
	float	prevFrameTime
) {
	// call base function
	InkdotEntity::onUpdateEnd(prevFrameTime);

	// apply gravity
	applySceneGravity(prevFrameTime);

	// update sprite animation
	sprite.updateAnimation(prevFrameTime);
}

// ===========================================================================
// renders entity sprites - called after background sprites are drawn
// ===========================================================================
void InkdotLegacyEntity::renderSprites()
{
	// draw sprite if active
	if(active) sprite.draw();
}


// extendable setters

// ===========================================================================
// set position of entity center using vector
// ===========================================================================
void InkdotLegacyEntity::setCenter(
	VECTOR2	pos
) {
	// if new position is equal to current position, exit early
	if (center == pos) return;

	// call base function to update colliders
	InkdotEntity::setCenter(pos);

	// update position for sprite
	sprite.setX(pos.x + sprite.getRelativeCenter()->x);
	sprite.setY(pos.y + sprite.getRelativeCenter()->y);
}

// ===========================================================================
// set entity rotation (in radians)
// ===========================================================================
void InkdotLegacyEntity::setRotation(
	float	r
) {
	// if new rotation is equal to current rotation, exit early
	if (rotation == r) return;

	// call base function to update colliders
	InkdotEntity::setRotation(r);

	// don't translate sprite offset according to rotation, since image
	// rotation is done at its center by graphics handler

	// update rotation for sprite
	sprite.setRadians(r);
}

// ===========================================================================
// set entity scale
// ===========================================================================
void InkdotLegacyEntity::setScale(
	float	s
) {
	// if new rotation is equal to current rotation, exit early
	if (scale == s) return;

	// call base function to update colliders
	InkdotEntity::setScale(s);

	// don't translate sprite offset according to rotation, since image
	// rotation is done at its center by graphics handler

	// update scale for sprite
	sprite.setScale(s);
}

// ===========================================================================
// set entity active status
// ===========================================================================
void InkdotLegacyEntity::setActive(
	bool	e
) {
	// call base function to update state
	InkdotEntity::setActive(e);
}

// ===========================================================================
// set entity visible state
// ===========================================================================
void InkdotLegacyEntity::setVisible(
	bool	e
) {
	// call base function to update state
	InkdotEntity::setVisible(e);

	// update visibility for sprite
	sprite.setVisible(e);
}
