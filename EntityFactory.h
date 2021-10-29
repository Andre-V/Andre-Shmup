#pragma once
#include "Entity.h"
#include "EntityInterfaces.h"
#include "EntityManager.h"

class EntityManager;
class Something;
class Player;

class EntityFactory
{
private:
	// To expose the EntityFactory's templates, I had to allow use
	// of EntityFactory directly.
	IEntityInsert& _entityInserter;
public:
	EntityFactory(IEntityInsert& entityManager)
		: _entityInserter(entityManager)
	{
		
	}
	template<typename E>
	Entity& make()
	{
		Entity* entity = new Entity;
		_entityInserter.insert(*entity);
		return *entity;
	}
	template<typename E, typename... ERest>
	Entity& make(ERest...)
	{
		return this->make<E>();
	}

	template<>
	Entity& make<Something>()
	{

	}
	template<>
	Entity& make<Player,int,int>(int t, int t2)
	{

	}

};