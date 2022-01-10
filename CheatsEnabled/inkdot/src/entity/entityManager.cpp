// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module:			Gameplay Programming
// Assignment1:		Cheats:Enabled
// Student Name:	Eugene Long - [Engine Designer]
// Student No.:		S10193060J
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// ===========================================================================
// InkdotEngine - Entity Manager Class Implementation
// ===========================================================================
// * This is a InkdotEngine component, derived from the standard GPP engine
//   templates provided by Ms. Lee Ching Yun, modified by Eugene Long.
// ===========================================================================

// include specification
#include "entityManager.h"

// ===========================================================================
// default constructor
// ===========================================================================
EntityManager::EntityManager():

	// manager states
	nextEntityId	(0),
	freedIds		(),
	entities		(),
	paused			(FALSE),
	searchArray		(),

	// debug
	debugText		(),

	// handlers
	scene			(NULL)
{}

// ===========================================================================
// default destructor
// ===========================================================================
EntityManager::~EntityManager()
{
	// safely delete all registered entities
	deleteAllEntities();
}


// public methods

// ===========================================================================
// initializes the entity manager instance
// ===========================================================================
void EntityManager::initialize(
	IScene*	owner
) {
	// store argument info into instance state
	scene = owner;

	// initialize shared debug text object
	if (!debugText.initialize(
		owner->getGraphics(),
		10,
		true,
		false,
		L"Terminal"
	)) throw(
		InkdotError(
			InkdotErrorType::FATAL_ERROR,
			"Error: Failed to initialize debug text handler!"
		)
		);
	debugText.setHighlight(CommonColors::BLACK);
	debugText.setTextColor(CommonColors::LIME);
}


// entity manager interface methods

// ===========================================================================
// adds an entity to the scene, returning the id of the new entity - this id 
// is guaranteed to remain constant throughout the scene lifecycle
// ===========================================================================
ENTITY_ID EntityManager::addEntity(
	InkdotEntity*	entity,
	int				zPos
) {
	// initialize entity id to next entity id
	ENTITY_ID entityId = nextEntityId;

	// check if any entity ids can be reused
	if (freedIds.size() > 0) {

		// take id from queue as this entity's new id
		entityId = freedIds.front();
		freedIds.pop();
	}

	// add entity to hashmap - no checks needed since id is autoincremented
	entities.emplace(
		entityId,
		entity
	);

	// bind entity to this entity manager
	entity->setEntityManager(this);

	// bind entity debug text to entity manager's debug text
	entity->setDebugText(&debugText);

	// save entity id to entity
	entity->setEntityId(entityId);

	// increment id for next entity
	nextEntityId++;

	// return recently added entity's id
	return entityId;
}

// ===========================================================================
// deletes an entity with the specified id from the scene
// ===========================================================================
void EntityManager::deleteEntity(
	ENTITY_ID	id
) {
	// attempt to get entity for id
	InkdotEntity* entityToDelete = getEntity(id);

	// if entity can't be found, return early
	if (!entityToDelete) return;

	// else, deallocate entity safely
	SAFE_DELETE(entityToDelete);

	// remove entity from entities
	entities.erase(id);

	// add id to freed id queue
	freedIds.push(id);
}

// ===========================================================================
// deletes all entities from the entity manager
// ===========================================================================
void EntityManager::deleteAllEntities()
{
	// safely delete all registered entities

	// setup entities iterator
	std::map<int, InkdotEntity*>::iterator it = entities.begin();

	// loop through iterator until all elements traversed
	while (it != entities.end()) {

		// delete mapped scene object at scene pointer
		SAFE_DELETE(it->second);

		// increment iterator
		it++;
	}
}


// entity manager interface getters

// ===========================================================================
// retrieves an entity with the specified id within the scene
// ===========================================================================
InkdotEntity* EntityManager::getEntity(
	ENTITY_ID	id
) {
	// if no entity can be found with the id, throw warning and return
	if (entities.count(id) < 1)
	{
		throw(
			InkdotError(
				InkdotErrorType::WARNING,
				"Warning: No entity with id: " + std::to_string(id) + "!"
			)
		);
		return NULL;
	}

	// else, return mapped entity
	return entities[id];
}

// ===========================================================================
// retrieves the index of the first entity with the specified tag
// ===========================================================================
std::vector<InkdotEntity*>* EntityManager::getEntitiesWithTag(
	std::string	tag,
	bool		onlyActive
) {
	// initialize search results
	searchArray.clear();

	// setup entities iterator
	std::map<int, InkdotEntity*>::iterator it = entities.begin();

	// loop through iterator until all elements traversed
	while (it != entities.end()) {

		// if active element filter is enabled and entity is inactive
		if (onlyActive && !it->second->isActive()) {

			// skip to next element
			it++;
			continue;
		}

		// if entity has tag, add entity to search results
		if (it->second->hasTag(tag)) searchArray.push_back(it->second);

		// increment iterator
		it++;
	}

	// return pointer to search array
	return &searchArray;
}

// ===========================================================================
// retrieves the index of the first entity with the specified tags
// ===========================================================================
std::vector<InkdotEntity*>* EntityManager::getEntitiesWithTags(
	std::string tags[],
	size_t		nTags,
	bool		onlyActive
) {
	// initialize search results
	searchArray.clear();

	// setup entities iterator
	std::map<int, InkdotEntity*>::iterator it = entities.begin();

	// loop through iterator until all elements traversed
	while (it != entities.end()) {

		// if active element filter is enabled and entity is inactive
		if (onlyActive && !it->second->isActive()) {

			// skip to next element
			it++;
			continue;
		}

		// loop through required tags
		for (size_t i = 0; i < nTags; i++) {

			// if any tag does not exist on the entity, check next entity
			if (!it->second->hasTag(tags[i])) break;
		}

		// else, if all tags exist on entity, add entity to search results
		searchArray.push_back(it->second);

		// increment iterator
		it++;
	}

	// return pointer to search array
	return &searchArray;
}


// entity-delegating methods

// ===========================================================================
// calls update() for all entities in the entity manager
// ===========================================================================
void EntityManager::updateAllEntities(
	float	prevFrameTime
) {
	// setup entities iterator
	std::map<int, InkdotEntity*>::iterator it = entities.begin();

	// loop through iterator until all elements traversed
	while (it != entities.end()) {

		// if not paused and entity is enabled
		if (!paused && it->second->isActive())

			// update entity
			it->second->update(prevFrameTime);

		// increment iterator
		it++;
	}
}

// ===========================================================================
// calls onUpdateEnd() for all entities in the entity manager
// ===========================================================================
void EntityManager::updateEndForAllEntities(
	float	prevFrameTime
) {
	// setup entities iterator
	std::map<int, InkdotEntity*>::iterator it = entities.begin();

	// loop through iterator until all elements traversed
	while (it != entities.end()) {

		// if not paused and entity is enabled
		if (!paused && it->second->isActive())
			
			// update entity
			it->second->onUpdateEnd(prevFrameTime);

		// increment iterator
		it++;
	}
}

// ===========================================================================
// handles ai for all entities in the entity manager
// ===========================================================================
void EntityManager::handleAIForAllEntities()
{
	// setup entities iterator
	std::map<int, InkdotEntity*>::iterator it = entities.begin();

	// loop through iterator until all elements traversed
	while (it != entities.end()) {

		// if not paused and entity is enabled
		if (!paused && it->second->isActive()) it->second->ai();

		// increment iterator
		it++;
	}
}

// ===========================================================================
// calls renderSprites() for all entities in the entity manager
// ===========================================================================
void EntityManager::renderSpritesForAllEntities()
{
	// setup entities iterator
	std::map<int, InkdotEntity*>::iterator it = entities.begin();

	// loop through iterator until all elements traversed
	while (it != entities.end()) {

		// call renderSprites() for entity
		it->second->renderSprites();

		// if debug enabled
		if (GameConstants::DEBUG_ON || it->second->debugEnabled()) 

			// call drawDebug() for entity
			it->second->drawDebug();

		// increment iterator
		it++;
	}
}

// ===========================================================================
// calls releaseAllGraphics() for all entities in the entity manager
// ===========================================================================
void EntityManager::releaseAllGraphicsForAllEntities()
{
	// setup entities iterator
	std::map<int, InkdotEntity*>::iterator it = entities.begin();

	// loop through iterator until all elements traversed
	while (it != entities.end()) {

		// call releaseAllGraphics() for entity
		it->second->releaseAllGraphics();

		// increment iterator
		it++;
	}
}

// ===========================================================================
// calls resetAllGraphics() for all entities in the entity manager
// ===========================================================================
void EntityManager::resetAllGraphicsForAllEntities()
{
	// setup entities iterator
	std::map<int, InkdotEntity*>::iterator it = entities.begin();

	// loop through iterator until all elements traversed
	while (it != entities.end()) {

		// call resetAllGraphics() for entity
		it->second->resetAllGraphics();

		// increment iterator
		it++;
	}
}
