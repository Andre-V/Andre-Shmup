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
	template<typename T>
	list<shared_ptr<Entity>> get()
	{
		//return _entityManager.get<
	}
};