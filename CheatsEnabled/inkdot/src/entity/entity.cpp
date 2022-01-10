// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module:			Gameplay Programming
// Assignment1:		Cheats:Enabled
// Student Name:	Eugene Long - [Engine Designer]
// Student No.:		S10193060J
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// ===========================================================================
// InkdotEngine - Entity Class Implementation
// ===========================================================================
// * This is a InkdotEngine component, derived from the standard GPP engine
//   templates provided by Ms. Lee Ching Yun, modified by Eugene Long.
// ===========================================================================

// include specification
#include "entity.h"

// ===========================================================================
// default constructor
// ===========================================================================
InkdotEntity::InkdotEntity():

	// entity states
	id			(0),
	center		(0, 0),
	rotation	(0),
	scale		(1),
	tags		(),
	debug		(FALSE),
	active		(TRUE),
	visible		(TRUE),

	// debug
	debugText	(NULL),

	// handlers
	entityManager	(NULL),

	// physics/kinematics
	mass			(1.0f),
	velocity		(0, 0),
	deltaV			(0, 0),
	gravityEnabled	(TRUE),
	gravity			(0),
	entityGravity	(0),
	rr				(0)
{}

// ===========================================================================
// default destructor
// ===========================================================================
InkdotEntity::~InkdotEntity() {}


// extendable entity methods

// ===========================================================================
// initializes the entity - called on scene initialization
// ===========================================================================
void InkdotEntity::initialize(
	float	initialX,
	float	initialY,
	float	_mass,
	float	_gravity
) {
	// move entity to initial position
	setCenter(initialX, initialY);

	// update physics states
	mass	= _mass;
	gravity	= _gravity;
}

// ===========================================================================
// updates the entity - called each frame after scene update() ends
// ===========================================================================
void InkdotEntity::onUpdateEnd(float prevFrameTime)
{
	// apply deltaV to entity and reset it
	setVelocity(velocity + deltaV);
	setDeltaV(VECTOR2(0, 0));
}


// collider methods

// ===========================================================================
// adds a circle collider to the entity instance
// ===========================================================================
void InkdotEntity::addCollider(
	std::string	colliderId,
	float		relX,
	float		relY,
	float		radius
) {
	// if collider identifier has been used already, throw warning and return
	if (colliders.count(colliderId) > 0)
	{
		throw(
			InkdotError(
				InkdotErrorType::WARNING,
				"Warning: Collider identifier has already been used!"
			)
		);
		return;
	}

	// else, register new collider - note that emplace constructs the new 
	// element in place, which is more appropriate compared to insert()
	colliders.emplace(
		colliderId,
		EntityCollider(
			center.x,
			center.y,
			relX,
			relY,
			radius
		)
	);
}

// ===========================================================================
// adds a box collider to the entity instance
// ===========================================================================
void InkdotEntity::addCollider(
	std::string	colliderId,
	float		relX,
	float		relY,
	RECT		edges
) {
	// if collider identifier has been used already, throw warning and return
	if (colliders.count(colliderId) > 0)
	{
		throw(
			InkdotError(
				InkdotErrorType::WARNING,
				"Warning: Collider identifier has already been used!"
			)
		);
		return;
	}

	// else, register new collider - note that emplace constructs the new 
	// element in place, which is more appropriate compared to insert()
	colliders.emplace(
		colliderId,
		EntityCollider(
			center.x,
			center.y,
			relX,
			relY,
			edges
		)
	);
}

// ===========================================================================
// retrieves the collider previously added using the specified identifier
// ===========================================================================
Collider* InkdotEntity::getCollider(
	std::string	colliderId
) {
	// if no element can be found with the id, throw warning and return
	if (colliders.count(colliderId) < 1)
	{
		throw(
			InkdotError(
				InkdotErrorType::WARNING,
				"Warning: No collider registered for id: " + colliderId + "!"
			)
		);
		return NULL;
	}

	// else, return collider for id
	return &colliders[colliderId];
}

// ===========================================================================
// removes the collider previously added using the specified identifier
// ===========================================================================
void InkdotEntity::removeCollider(
	std::string	colliderId
) {
	// if no element can be found with the id, throw warning and return
	if (colliders.count(colliderId) < 1)
	{
		throw(
			InkdotError(
				InkdotErrorType::WARNING,
				"Warning: No collider registered for id: " + colliderId + "!"
			)
		);
		return;
	}

	// else, erase collider for id from colliders - also frees memory
	colliders.erase(colliderId);
}

// ===========================================================================
// checks if the specified collider is colliding with any entity that has
// the specified tag.
// ===========================================================================
bool InkdotEntity::collidesWithTaggedEntities(
	std::string		colliderId,
	std::string		tag,
	COLLISION_LIST&	collisionList,
	bool			onlyActive
) {
	// attempt to get collider for id. if it fails, exit early
	Collider* c = getCollider(colliderId); if (!c) return false;

	// search entity manager for entities with tags
	std::vector<InkdotEntity*>* res = entityManager->getEntitiesWithTag(tag);

	// if no results, exit early
	if (res->size() < 1) return false;

	// else initialize states for collision checks
	VECTOR2 cV{0, 0};
	collisionList.clear();

	// iterate through each found entity
	for (size_t i = 0; i < res->size(); i++) {

		// get iterator for entity collider map
		std::unordered_map<std::string, EntityCollider>::iterator it =
			(*res)[i]->getAllColliders()->begin();

		// iterate through all colliders
		while (it != (*res)[i]->getAllColliders()->end()) {

			// check for collision
			if (c->collidesWith(it->second, cV)) {

				// if it does collide, save collision info to collision list
				collisionList.push_back(std::make_tuple(
					cV,
					it->first,
					(*res)[i]->getEntityId()
				));
			}

			// increment iterator
			it++;
		}
	}

	// return collision status
	return collisionList.size() > 0;
}


// physics methods

// ===========================================================================
// handle entity bounce after collision with another entity
// ===========================================================================
void InkdotEntity::bounce(
	VECTOR2&		collisionVector,
	InkdotEntity&	oth
) {
	// get difference in velocity between objects (AB = OB - OA)
	VECTOR2 vDiff = *oth.getVelocity() - velocity;

	// copy collision vector to working state to avoid recalculating cV
	VECTOR2 cUV = collisionVector;

	// derive direction vector for collision vector
	Vector2Math::Normalize(&cUV);

	// get dot product between collision unit vector and velocity difference
	float cUVdotVdiff = Vector2Math::Dot(&cUV, &vDiff);

	// initialize mass ratio to 2 - 
	float massRatio = 2.0f;

	// if this entity's mass is non-negligible relative to other entity's
	// mass, calculate the exact mass ratio
	if (mass != 0) massRatio = (oth.getMass() / (mass + oth.getMass()));

	// if entities are already moving apart, bounce is likely to have been 
	// previously called, but objects are still colliding.

	// check if entities are moving apart (dot product > 0)
	if (cUVdotVdiff > 0)
	{
		// if so, move entities apart along collision vector
		setCenter(
			getCenter()->x - cUV.x * massRatio,
			getCenter()->y - cUV.y * massRatio
		);
	}
	else
		// else, add bounce force (magnitude * direction) to deltaV
		deltaV += ((massRatio * cUVdotVdiff) * cUV);
}

// ===========================================================================
// apply gravitational force exerted by another entity to this entity
// ===========================================================================
void InkdotEntity::applyGravityFromEntity(
	InkdotEntity*	oth,
	float			prevFrameTime
) {
	// if either entity's gravity is not enabled, exit early
	if (!gravityEnabled || !oth->getGravityEnabled()) return;

	// let the position vector to the center of this entity be OA
	// let the position vector to the center of the other entity be OB

	//                              mass of obj 1 * mass of obj 2
	// gravitational force = G * ---------------------------------- ,
	//                                          r^2
	
	// r^2 = (distance between obj 1 and 2)^2 = (Ax - Bx)^2 + (Ay - By)^2

	// initialize gV to AB (OB - OA), the unnormalized gravity unit vector
	VECTOR2 gV(
		oth->getCenter()->x - center.x,
		oth->getCenter()->y - center.y
	);
	
	// r^2 = (Ax - Bx)^2 + (Ay - By)^2 = ABx^2 + ABy^2
	rr = gV.x * gV.x + gV.y * gV.y;

	// calculate the magnitude of the gravitational force
	entityGravity = PhysicsConstants::GRAVITY * oth->getMass() * mass * rr;

	// normalise gravitational vector to get the gravitational unit vector
	Vector2Math::Normalize(&gV);

	// apply magnitude of the exerted gravitational force to the unit vector
	// to get the actual gravity vector (frametime used to account for frame-
	// based calculation)
	gV *= entityGravity * prevFrameTime;

	// apply gravity to current entity's velocity
	velocity += gV;
}

// ===========================================================================
// apply standard gravitational force on entity
// ===========================================================================
void InkdotEntity::applySceneGravity(
	float	prevFrameTime
) {
	// if gravity is disabled for this entity, exit early
	if (!gravityEnabled) return;

	// calculate gravitational vector (downward direction, y is increasing)
	// gVy = mass * gravity
	VECTOR2 gV(
		0,
		mass * PhysicsConstants::SCENE_GRAVITY * gravity * prevFrameTime
	);

	// apply gravity to current entity's velocity
	velocity += gV;
}

// ===========================================================================
// apply entity's current velocity to entity position
// ===========================================================================
void InkdotEntity::applyVelocity(
	float prevFrameTime
) {
	// update position according to velocity and current frametime
	setCenter(center + velocity * prevFrameTime);
}


// convenience methods

// ===========================================================================
// update component center according to entity rotation and scale
// ===========================================================================
void InkdotEntity::updateComponentCenter(
	EntityComponent&	component,
	bool				applyRotation
) {
	// update component center according to entity rotation and scale

	// let center of entity be point A
	// let center of component be point B
	// let relative position of component from entity be AB

	// O-------A
	//  \     / \
	//   \   /\_/\
	//    \ /  0  \
	//     B'      B

	// if B' is the rotated vector, find OB' :

	// store relative position vector for easy access
	VECTOR2 AB = *component.getRelativeCenter() * scale;

	// if no rotation needed, update component position and exit
	if (!applyRotation) return component.setComponentPosition(center + AB);

	// apply rotation matrix to AB to get AB'
	// [ cos0, -sin0 ][ x ] -> [ x cos0 - y sin0 ]
	// [ sin0,  cos0 ][ y ]    [ x sin0 + y cos0 ]

	VECTOR2 ABPrime(
		AB.x * cos(rotation) - AB.y * sin(rotation),
		AB.x * sin(rotation) + AB.y * cos(rotation)
	);

	// update component center to OB' (OA + AB')
	component.setComponentPosition(center + ABPrime);
}


// debug methods

// ===========================================================================
// draws an outline for the specified collider
// ===========================================================================
void InkdotEntity::drawColliderOutline(
	Collider*			collider, 
	const COLOR_ARGB&	col,
	const float&		z
) {
	// only render if graphics can be attached
	Graphics* g; if (!(g = getGraphics())) return;

	// if collider is null, return early
	if (!collider) return;

	// use corners to draw an outline for the collider
	VECTOR2 c[4]{};

	// get updated corners
	collider->updateCorners();

	// set corners to updated corners
	c[0] = *collider->getColliderCorner(0);
	c[1] = *collider->getColliderCorner(1);
	c[2] = *collider->getColliderCorner(2);
	c[3] = *collider->getColliderCorner(3);

	// normalize z
	float nZ = NORMALIZED_Z(z);

	// set up vertex buffers for line strip
	Vertex vertices[5]
	{
		Vertex(c[0].x, c[0].y, nZ, col, 0.0f, 0.0f),
		Vertex(c[1].x, c[1].y, nZ, col, 1.0f, 0.0f),
		Vertex(c[2].x, c[2].y, nZ, col, 0.0f, 1.0f),
		Vertex(c[3].x, c[3].y, nZ, col, 1.0f, 1.0f),
		Vertex(c[0].x, c[0].y, nZ, col, 0.0f, 0.0f)
	};

	// draw primitive
	g->drawPrimitive(vertices, 5, D3DPT_LINESTRIP, 4);
}

// ===========================================================================
// draws a bounding box for the specified collider
// ===========================================================================
void InkdotEntity::drawColliderBoundingBox(
	Collider*			collider, 
	const COLOR_ARGB&	col,
	const float&		z
) {
	// only render if graphics can be attached
	Graphics* g; if (!(g = getGraphics())) return;

	// use only two corners to draw bounding boxes
	VECTOR2 c[2];

	// get updated corners
	collider->updateCorners();

	// initialize corners to corner 0
	c[0] = *collider->getColliderCorner(0);
	c[1] = *collider->getColliderCorner(0);

	// get min and max corners for bounding box
	for (int i = 1; i < 4; i++) {

		// get corner from collider
		const VECTOR2* corner = collider->getColliderCorner(i);
		
		// set corners min and max accordingly
		if (corner->x < c[0].x) c[0].x = corner->x;
		if (corner->y < c[0].y) c[0].y = corner->y;
		if (corner->x > c[1].x) c[1].x = corner->x;
		if (corner->y > c[1].y) c[1].y = corner->y;
	}

	// normalize z
	float nZ = NORMALIZED_Z(z);

	// set up vertex buffers for line strip
	Vertex vertices[5]
	{
		Vertex(c[0].x, c[0].y, nZ, col, 0.0f, 0.0f),
		Vertex(c[1].x, c[0].y, nZ, col, 1.0f, 0.0f),
		Vertex(c[1].x, c[1].y, nZ, col, 0.0f, 1.0f),
		Vertex(c[0].x, c[1].y, nZ, col, 1.0f, 1.0f),
		Vertex(c[0].x, c[0].y, nZ, col, 0.0f, 0.0f)
	};

	// draw primitive
	g->drawPrimitive(vertices, 5, D3DPT_LINESTRIP, 4);
}

// ===========================================================================
// draws debug information for the entity
// ===========================================================================
void InkdotEntity::drawDebug()
{
	// draw hitboxes for all colliders

	// create collider iterator
	std::unordered_map<std::string, EntityCollider>::iterator it =
		colliders.begin();

	// iterate through all colliders
	while (it != colliders.end()) {

		// draw
		drawColliderOutline(&it->second, CommonColors::LIME, 0.02f);
		drawColliderBoundingBox(&it->second, CommonColors::RED, 0.01f);
		debugText->print(
			std::to_wstring(id), 
			(int) center.x, 
			(int) center.y,
			0.03f
		);

		// increment iterator
		it++;
	}
}


// setters

// ===========================================================================
// set position of entity center using vector
// ===========================================================================
void InkdotEntity::setCenter(
	VECTOR2	pos
) {
	// if new position is equal to current position, exit early
	if (center == pos) return;


	// else, update positions for entity and entity components
	
	// update position of center for entity
	center = pos;

	// update collider positions

	// create collider iterator
	std::unordered_map<std::string, EntityCollider>::iterator it = 
		colliders.begin();

	// iterate through all colliders
	while (it != colliders.end()) {

		// update position for each collider
		it->second.setColliderCenter(pos + *it->second.getRelativeCenter());

		// increment iterator
		it++;
	}
}

// ===========================================================================
// set entity rotation (in radians)
// ===========================================================================
void InkdotEntity::setRotation(
	float	r
) {
	// if new rotation is equal to current rotation, exit early
	if (rotation == r) return;


	// else, update rotations for entity and entity components

	// update rotation for entity
	rotation = r;

	// update collider rotations

	// create collider iterator
	std::unordered_map<std::string, EntityCollider>::iterator it =
		colliders.begin();

	// iterate through all colliders
	while (it != colliders.end()) {

		// if collider is offset from center
		if (*it->second.getRelativeCenter() != VECTOR2(0,0))
		{
			// update collider center
			updateComponentCenter(it->second, true);
		}

		// update collider rotation
		it->second.setColliderAngle(r);

		// increment iterator
		it++;
	}
}

// ===========================================================================
// set entity scale
// ===========================================================================
void InkdotEntity::setScale(
	float	s
) {
	// if new scale is equal to old scale, exit early
	if (rotation == s) return;


	// else, update rotations for entity and entity components

	// update rotation for entity
	scale = s;

	// update collider scales

	// create collider iterator
	std::unordered_map<std::string, EntityCollider>::iterator it =
		colliders.begin();

	// iterate through all colliders
	while (it != colliders.end()) {

		// if collider is offset from center
		if (*it->second.getRelativeCenter() != VECTOR2(0, 0))
		{
			// update collider center
			updateComponentCenter(it->second, true);
		}

		// update collider scale
		it->second.setColliderScale(s);

		// increment iterator
		it++;
	}
}

// ===========================================================================
// set entity active status
// ===========================================================================
void InkdotEntity::setActive(
	bool	enabled
) {
	// update states for entity and entity components

	// update rotation for entity
	active = enabled;

	// update collider enabled states

	// create collider iterator
	std::unordered_map<std::string, EntityCollider>::iterator it =
		colliders.begin();

	// iterate through all colliders
	while (it != colliders.end()) {

		// update collider state
		it->second.setColliderEnabled(enabled);

		// increment iterator
		it++;
	}
}
