#pragma once
#include <memory>
#include <list>
#include "EntityManager.h"

using namespace std;
// Stuff Systems are allowed to use to interact with entities
// I would've used interfaces but virtual templates don't let me

// IT'S GETTING ANNOYING USING THIS SO I REMOVED IT.
/*
class EntityRequests
{
private:
	shared_ptr<EntityManager> _entityManager;
public:
	EntityRequests(shared_ptr<EntityManager> entityManager) :
		_entityManager(entityManager)
	{

	}
	EntityRequests(EntityManager& entityManager) :
		EntityRequests(shared_ptr<EntityManager>(&entityManager))
	{

	}
	template<typename T, typename... TRest>
	list<shared_ptr<Entity>> get()
	{
		return (*_entityManager).get<T, TRest...>();
	}
	template<typename E>
	Entity& make()
	{
		return (*_entityManager).make<E>();
	}
	template<typename E, typename... ERest>
	Entity& make(ERest...)
	{
		return (*_entityManager).make<E>(ERest...);
	}
	void remove(Entity& entity)
	{
		(*_entityManager).remove(entity);
	}
	void remove(shared_ptr<Entity> entity)
	{
		(*_entityManager).remove(entity);
	}
	EntityFactory& getFactory()
	{
		return (*_entityManager).entityFactory;
	}
};*/