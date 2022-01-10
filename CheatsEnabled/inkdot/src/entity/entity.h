// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module:			Gameplay Programming
// Assignment1:		Cheats:Enabled
// Student Name:	Eugene Long - [Engine Designer]
// Student No.:		S10193060J
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// ===========================================================================
// InkdotEngine - Entity Class Specification
// ===========================================================================
// * This is a InkdotEngine component, derived from the standard GPP engine
//   templates provided by Ms. Lee Ching Yun, modified by Eugene Long.
// ===========================================================================

#ifndef _INKDOT_ENTITY_H
#define _INKDOT_ENTITY_H

// include necessary headers
#include "../graphics/image.h"
#include "../graphics/text.h"
#include "../input/input.h"
#include "../interfaces/IEntityManager.h"
#include "collider.h"
#include "entityComponent.h"
#include <string>
#include <unordered_map>
#include <set>

// typedefs

// contains collision information, such as the collision vector, collider id
// and collided entity id for a single collision.
typedef std::tuple<VECTOR2, std::string, ENTITY_ID> COLLISION_LIST_ELEMENT;

// collision check result vector - contains collision information for
// multiple collisions. refer to COLLISION_LIST_ELEMENT for specifics.
typedef std::vector<COLLISION_LIST_ELEMENT> COLLISION_LIST;


// related constructs

class EntityCollider : public Collider, public EntityComponent
{
public:

	// default constructor
	EntityCollider():

		// base collider constructor
		Collider(),

		// base entity component constructor
		EntityComponent(0, 0)
	{}

	// circle collider constructor
	EntityCollider(
		float			entX,
		float			entY,
		float			relX,
		float			relY,
		float			radius
	) :
		// base collider constructor
		Collider		(entX + relX, entY + relY, radius),

		// base entity component constructor
		EntityComponent	(relX, relY)
	{}

	// box collider constructor
	EntityCollider(
		float			entX,
		float			entY,
		float			relX,
		float			relY,
		RECT			edges
	) :
		// base collider constructor
		Collider	(entX + relX, entY + relY, edges),

		// base entity component constructor
		EntityComponent(relX, relY)
	{}

	// destructor
	~EntityCollider() {}


	// component setters

	// sets the position of the component to pos
	virtual void setComponentPosition(VECTOR2 pos)
	{ setColliderCenter(pos); }
};


// class specification

class InkdotEntity
{
protected:

	// entity states

	// unique identifier of the entity - should only be set when bound to an
	// entity manager
	ENTITY_ID id;

	// position of entity's center according to screen coordinates
	VECTOR2 center;

	// clockwise angle of rotation of entity (in radians)
	float rotation;

	// scale of the entity
	float scale;

	// tags for the entity - use these to identify aspects of the entity
	std::set<std::string> tags;

	// specifies if debug mode is on
	bool debug;

	// specifies if entity is active
	bool active;

	// specifies if entity is visible
	bool visible;


	// debug

	// pointer to debug text object - provided by entity manager
	Text* debugText;


	// handlers

	// the bound entity manager associated with this instance - set when the
	// entity is added to an entity manager through setEntityManager()
	IEntityManager* entityManager;


	// collider states

	// string to entity collider map
	std::unordered_map<std::string, EntityCollider> colliders;


	// physics/kinematics states

	// mass of entity
	float mass;

	// velocity of entity (directional force)
	VECTOR2 velocity;

	// change in velocity, applied during next update() call
	VECTOR2 deltaV;

	// specifies if gravity is currently enabled for the instance
	bool gravityEnabled;

	// percentage of scene gravity applied to entity on applySceneGravity()
	float gravity;

	// working state - gravitational force exerted by another entity on this
	float entityGravity;

	// working state - radius squared for gravitational calculations
	float rr;

public:

	// default constructor
	InkdotEntity();

	// default destructor
	~InkdotEntity();


	// tag methods

	// adds a tag to the entity
	void addTag(std::string tag)
	{ tags.insert(tag); }

	// removes a tag from the entity
	void removeTag(std::string tag)
	{ tags.erase(tag); }

	// checks if the specified tag exists on this entity
	bool hasTag(std::string tag)
	{ return tags.count(tag); }


	// extendable entity methods

	// initializes the entity - called on scene initialization
	virtual void initialize(
		float	initialX,
		float	initialY,
		float	_mass		= 1.0f,
		float	_gravity	= 1.0f
	);

	// prepares the entity for deletion - called before entity is destroyed
	virtual void cleanup() {}

	// updates the entity - called each frame before scene update() starts
	virtual void update(float prevFrameTime) {}

	// updates the entity - called each frame after scene update() ends
	virtual void onUpdateEnd(float prevFrameTime);

	// handles artificial intelligence for the entity - called each frame
	virtual void ai() {}

	// renders entity sprites - called after background sprites are drawn
	virtual void renderSprites() {}

	// releases all memory reserved for graphics objects so that the bound
	// graphics device can be reset - called when the graphics device is lost
	virtual void releaseAllGraphics() {}

	// recreates and restores all graphics objects - called when the lost
	// graphics device is restored
	virtual void resetAllGraphics() {}


	// collider methods

	/// <summary>
	/// Adds a circle collider to the entity instance
	/// </summary>
	/// <param name="colliderId">
	/// : The unique string identifier for this collider.
	/// </param>
	/// <param name="relX">
	/// : The relative x-coordinate of the collider's center to the center of 
	///   the entity.
	/// </param>
	/// <param name="relY">
	/// : The relative y-coordinate of the collider's center to the center of 
	///   the entity.
	/// </param>
	/// <param name="radius">
	/// : The radius of the circle collider.
	/// </param>
	void addCollider(
		std::string	colliderId,
		float		relX,
		float		relY,
		float		radius
	);

	/// <summary>
	/// Adds a box collider to the entity instance
	/// </summary>
	/// <param name="colliderId">
	/// : The unique string identifier for this collider.
	/// </param>
	/// <param name="relX">
	/// : The relative x-coordinate of the collider's center to the center of 
	///   the entity.
	/// </param>
	/// <param name="relY">
	/// : The relative y-coordinate of the collider's center to the center of 
	///   the entity.
	/// </param>
	/// <param name="edges">
	/// : The distance to each edge of the box, relative to its center.
	/// </param>
	void addCollider(
		std::string	colliderId,
		float		relX,
		float		relY,
		RECT		edges
	);

	// retrieves the collider previously added using the specified identifier
	Collider* getCollider(
		std::string colliderId
	);

	// retrieves the collider map for the entity
	std::unordered_map<std::string, EntityCollider>* getAllColliders()
	{ return &colliders; }

	// removes the collider previously added using the specified identifier
	void removeCollider(
		std::string colliderId
	);

	/// <summary>
	/// Checks if the specified collider is colliding with any entity that has
	/// the specified tag.
	/// </summary>
	/// <param name="colliderId">
	/// : The identifier of the current entity's collider to check with.
	/// </param>
	/// <param name="tag">
	/// : The entity tag to filter entities for collision checks by.
	/// </param>
	/// <param name="collisionList">
	/// : Output. A reference to a vector that will contain collision
	///   information. Refer to the COLLISION_LIST type for details.
	/// </param>
	/// <param name="onlyActive">
	/// : Set this to false to also check for collisions against inactive
	///    entities. Defaults to true.
	/// </param>
	/// <returns>
	/// : Returns true if any collision did happen, and false otherwise
	/// </returns>
	bool collidesWithTaggedEntities(
		std::string		colliderId,
		std::string		tag,
		COLLISION_LIST& collisionList,
		bool			onlyActive		= TRUE
	);


	// physics methods

	// handle entity bounce after collision with another entity
	void bounce(
		VECTOR2&		collisionVector,
		InkdotEntity&	oth
	);

	// apply gravitational force exerted by another entity to this entity
	void applyGravityFromEntity(
		InkdotEntity*	oth,
		float			frameTime
	);

	// apply standard gravitational force on entity
	void applySceneGravity(float prevFrameTime);

	// apply entity's current velocity to entity position
	void applyVelocity(float prevFrameTime);


	// convenience methods
	
	// update component center according to entity rotation and scale
	void updateComponentCenter(
		EntityComponent&	component,
		bool				applyRotation
	);


	// debug methods

	// draws an outline for the specified collider
	void drawColliderOutline(
		Collider*			collider,
		const COLOR_ARGB&	col,
		const float&		z		= 0.1f
	);

	// draws an outline for the specified collider
	void drawColliderOutline(
		const std::string&	id,
		const COLOR_ARGB&	col,
		const float&		z		= 0.1f
	)
	{ drawColliderOutline(getCollider(id), col, z); }

	// draws a bounding box for the specified collider
	void drawColliderBoundingBox(
		Collider*			collider, 
		const COLOR_ARGB&	col,
		const float&		z		= 0.1f
	);

	// draws a bounding box for the specified collider
	void drawColliderBoundingBox(
		const std::string&	id,
		const COLOR_ARGB&	col,
		const float&		z		= 0.1f
	)
	{ drawColliderBoundingBox(getCollider(id), col, z); }

	// draws debug information for the entity
	virtual void drawDebug();


	// getters

	// get entity id
	ENTITY_ID getEntityId() const
	{ return id; }

	// get entity debug status
	bool debugEnabled()
	{ return debug; }

	// get entity active status
	bool isActive()
	{ return active; }

	// return position of entity center
	const VECTOR2* getCenter() const
	{ return &center; }

	// return entity rotation
	float getRotation() const
	{ return rotation; }

	// return mass
	float getMass() const
	{ return mass; }

	// return velocity vector
	const VECTOR2* getVelocity() const
	{ return &velocity; }

	// return deltaV vector
	const VECTOR2* getDeltaV() const
	{ return &deltaV; }

	// return gravity enabled state
	bool getGravityEnabled() const
	{ return gravityEnabled; }

	// return gravity
	float getGravity() const
	{ return gravity; }

	// return pointer to graphics handler
	Graphics* getGraphics() const
	{ 
		if (entityManager)	return entityManager->getGraphics();
		else				return NULL;
	}

	// return pointer to input handler
	Input* getInput() const
	{
		if (entityManager)	return entityManager->getInput();
		else				return NULL;
	}


	// extendable setters

	// set position of entity center using vector
	virtual void setCenter(VECTOR2 pos);

	// set entity rotation (in radians)
	virtual void setRotation(float r);

	// set entity scale
	virtual void setScale(float s);

	// set entity active status
	virtual void setActive(bool enabled);

	// set entity visible state
	virtual void setVisible(bool e)
	{ visible = e; }

	
	// setters

	// set entity id
	void setEntityId(ENTITY_ID _id)
	{ id = _id; }

	// set entity debug status
	void setDebug(bool enabled)
	{ debug = enabled; }

	// set position of entity center 
	void setCenter(float x, float y)
	{ setCenter(VECTOR2(x, y)); }

	// set mass
	void setMass(float m)
	{ mass = m; }

	// set velocity
	void setVelocity(VECTOR2 v)
	{ velocity = v; }

	// set x-velocity
	void setVelocityX(float x)
	{ velocity.x = x; }

	// set y-velocity
	void setVelocityY(float y)
	{ velocity.y = y; }

	// set delta velocity, which is added to velocity on next update() call
	void setDeltaV(VECTOR2 dv)
	{ deltaV = dv;}

	// set gravity enabled state
	void getGravityEnabled(bool enabled)
	{ gravityEnabled = enabled; }

	// set gravity
	void setGravity(float g)
	{ gravity = g; }

	// set entity manager
	void setEntityManager(IEntityManager* em)
	{ entityManager = em; }

	// set debug text handler
	void setDebugText(Text* t)
	{ debugText = t; }
};

#endif // !_INKDOT_ENTITY_H