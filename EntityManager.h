#pragma once
#include <memory>
#include <list>
#include <queue>
#include "Entity.h"
#include "EntityInterfaces.h"
#include "EntityFactory.h"

using namespace std;

class EntityManager : IEntityInsert
{
private:
	list<shared_ptr<Entity>> _entities;
	queue<shared_ptr<Entity>> _newEntities;
public:
	EntityFactory entityFactory;
	EntityManager() : entityFactory(*this)
	{

	}
	// return entities matching desired components
	template<typename T, typename... TRest>
	list<shared_ptr<Entity>> get()
	{
		list<shared_ptr<Entity>> matchingEntities;
		for (shared_ptr<Entity> entity : _entities)
		{
			if ((*entity).has<T, TRest...>() && (*entity).active)
			{
				matchingEntities.push_back(entity);
			}
		}
		return matchingEntities;
	}
	list<shared_ptr<Entity>> get()
	{
		return _entities;
	}
	template<typename T, typename... TRest>
	shared_ptr<Entity> getSingle()
	{
		for (shared_ptr<Entity> entity : _entities)
		{
			if ((*entity).has<T, TRest...>())
			{
				return entity;
			}
		}
		return nullptr;
	}
	template<typename E>
	Entity& make()
	{
		Entity& entity = entityFactory.make<E>();
		//_newEntities.push(shared_ptr<Entity>(&entity));
		return entity;
	}
	void update()
	{
		// remove entities flagged as noexistant
		for (auto iter = _entities.begin(); iter != _entities.end();)
		{
			if (!(**iter).exists)
			{
				iter = _entities.erase(iter);
			}
			else
			{
				++iter;
			}
		}
		// append new entities
		while (!_newEntities.empty())
		{
			_entities.push_back(_newEntities.front());
			_newEntities.pop();
		}
	}
	void insert(Entity& entity)
	{
		_newEntities.push(shared_ptr<Entity>(&entity));
	}
	void remove(Entity& entity)
	{
		entity.exists = false;
	}
	void remove(shared_ptr<Entity> entity)
	{
		this->remove(*entity);
	}
	// debug methods
	int numEntities()
	{
		return _entities.size();
	}
};