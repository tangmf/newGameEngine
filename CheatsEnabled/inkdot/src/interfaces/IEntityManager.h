// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module:			Gameplay Programming
// Assignment1:		Cheats:Enabled
// Student Name:	Eugene Long - [Engine Designer]
// Student No.:		S10193060J
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// ===========================================================================
// InkdotEngine - Entity Manager Interface Specification
// ===========================================================================
// * This is a InkdotEngine component, derived from the standard GPP engine
//   templates provided by Ms. Lee Ching Yun, modified by Eugene Long.
// ===========================================================================

#ifndef _INKDOT_ENTITYMANAGERINTERFACE_H
#define _INKDOT_ENTITYMANAGERINTERFACE_H

// import necessary headers
#include "../graphics/graphics.h"
#include "../input/input.h"
#include <string>
#include <vector>

// forward declarations

class InkdotEntity;

// typedefs

typedef int ENTITY_ID;


// class specification

class IEntityManager
{
public:

	// abstract methods to be implemented by child classes

	// should add an entity to the scene, returning the id of the entity - 
	// this id is guaranteed to remain constant throughout the scene lifecycle
	virtual ENTITY_ID addEntity(
		InkdotEntity*	entity, 
		int				zPos	= 0
	) = 0;

	// should delete an entity with the specified id from the scene
	virtual void deleteEntity(ENTITY_ID id) = 0;

	// deletes all entities from the entity manager
	virtual void deleteAllEntities() = 0;

	// should get an entity with the specified id within the scene
	virtual InkdotEntity* getEntity(ENTITY_ID id) = 0;

	// retrieves the entities that contain the specified tag
	virtual std::vector<InkdotEntity*>* getEntitiesWithTag(
		std::string tag,
		bool		onlyActive	= TRUE
	) = 0;

	// retrieves the entities that contain the specified tags
	virtual std::vector<InkdotEntity*>* getEntitiesWithTags(
		std::string tags[],
		size_t		nTags,
		bool		onlyActive	= TRUE
	) = 0;


	// abstract getters to be implemented by child classes

	// should return a pointer to the current graphics handler
	virtual Graphics* getGraphics() const = 0;

	// should return a pointer to the current input handler
	virtual Input* getInput() const = 0;
};

#endif // !_INKDOT_ENTITYMANAGERINTERFACE_H