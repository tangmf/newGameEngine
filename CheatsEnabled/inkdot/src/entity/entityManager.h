// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module:			Gameplay Programming
// Assignment1:		Cheats:Enabled
// Student Name:	Eugene Long - [Engine Designer]
// Student No.:		S10193060J
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// ===========================================================================
// InkdotEngine - Entity Manager Class Specification
// ===========================================================================
// * This is a InkdotEngine component, derived from the standard GPP engine
//   templates provided by Ms. Lee Ching Yun, modified by Eugene Long.
// ===========================================================================

#ifndef _INKDOT_ENTITYMANAGER_H
#define _INKDOT_ENTITYMANAGER_H

// import necessary headers
#include "../graphics/graphics.h"
#include "../input/input.h"
#include "../interfaces/IEntityManager.h"
#include "../interfaces/IScene.h"
#include "entity.h"
#include <vector>
#include <map>
#include <queue>


// class specification

class EntityManager : public IEntityManager
{
private:

	// manager states

	// entity id counter
	int nextEntityId = 0;

	// contains entity ids that were previously deleted and now free to use
	std::queue<ENTITY_ID> freedIds;

	// entity container
	std::map<ENTITY_ID, InkdotEntity*> entities;

	// specifies if all entities managed by this instance should stop updating
	bool paused;

	// working state - contains entity search results
	std::vector<InkdotEntity*> searchArray;


	// debug

	// shared debug text handler
	Text debugText;


	// handlers

	// pointer to bound scene manager instance
	IScene* scene;

public:

	// default constructor
	EntityManager();

	// default destructor
	~EntityManager();


	// methods

	/// <summary>
	/// Initializes the EntityManager instance
	/// </summary>
	/// <param name="owner">
	/// : A pointer to the Scene instance which owns this entity manager
	/// </param>
	void initialize(IScene* owner);


	// IEntityManager methods

	// adds an entity to the scene, returning the id of the new entity - 
	// this id is guaranteed to remain constant throughout the scene lifecycle
	virtual ENTITY_ID addEntity(
		InkdotEntity*	entity, 
		int				zPos	= 0
	);

	// deletes an entity with the specified id from the scene
	virtual void deleteEntity(ENTITY_ID id);

	// deletes all entities from the entity manager
	virtual void deleteAllEntities();

	// retrieves an entity with the specified id within the scene
	virtual InkdotEntity* getEntity(ENTITY_ID id);

	// retrieves the entities that contain the specified tag
	virtual std::vector<InkdotEntity*>* getEntitiesWithTag(
		std::string tag,
		bool		onlyActive = TRUE
	);

	// retrieves the entities that contain the specified tags
	virtual std::vector<InkdotEntity*>* getEntitiesWithTags(
		std::string tags[],
		size_t		nTags,
		bool		onlyActive = TRUE
	);


	// entity-delegating methods

	// calls update() for all entities in the entity manager
	void updateAllEntities(float prevFrameTime);

	// calls onUpdateEnd() for all entities in the entity manager
	void updateEndForAllEntities(float prevFrameTime);

	// handles ai for all entities in the entity manager
	void handleAIForAllEntities();

	// calls renderSprites() for all entities in the entity manager
	void renderSpritesForAllEntities();

	// calls releaseAllGraphics() for all entities in the entity manager
	void releaseAllGraphicsForAllEntities();

	// calls resetAllGraphics() for all entities in the entity manager
	void resetAllGraphicsForAllEntities();


	// IEntityManager getters

	// returns a pointer to the current graphics handler
	virtual Graphics* getGraphics() const
	{
		if (scene)	return scene->getGraphics();
		else		return NULL;
	}

	// returns a pointer to the current input handler
	virtual Input* getInput() const
	{
		if (scene)	return scene->getInput();
		else		return NULL;
	}


	// getters

	// get the current paused state for the entity manager
	bool getPaused() const { return paused; }


	// setters

	// sets the current paused state for the entity manager
	void setPaused(bool pause) { paused = pause; }
};

#endif // !_INKDOT_ENTITYMANAGER_H