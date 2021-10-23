#pragma once
#include <memory>
#include <list>
#include "EntityManager.h"

using namespace std;
// Stuff Systems are allowed to use to interact with entities
// I would've used interfaces but virtual templates don't let me
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
	void remove(Entity& entity)
	{
		(*_entityManager).remove(entity);
	}
	void remove(shared_ptr<Entity> entity)
	{
		(*_entityManager).remove(entity);
	}
};