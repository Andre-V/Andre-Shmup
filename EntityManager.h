#pragma once
#include <memory>
#include <list>
#include <queue>
#include <map>
#include "Entity.h"
#include "EntityInterfaces.h"
#include "EntityFactory.h"

using namespace std;

class EntityManager : IEntityInsert
{
private:
	list<shared_ptr<Entity>> _entities;
	queue<shared_ptr<Entity>> _newEntities;
	std::map<vector<bool>, list<shared_ptr<Entity>>> _cache;
	std::map<shared_ptr<Entity>, list<vector<bool>>> _reverseIndex;
public:
	// return entities matching desired components
	template<typename T, typename... TRest>
	list<shared_ptr<Entity>> get()
	{
		vector<bool> key = CmpMngr::getKey<T, TRest...>();
		if (_cache.count(key) != 0)
		{
			return _cache[key];
		}
		list<shared_ptr<Entity>> matchingEntities;
		for (shared_ptr<Entity> entity : _entities)
		{
			if ((*entity).has<T, TRest...>() && (*entity).active)
			{
				matchingEntities.push_back(entity);
				_cache[key].push_back(entity);
				_reverseIndex[entity].push_back(key);
			}
		}
		return matchingEntities;
	}
	list<shared_ptr<Entity>> get()
	{
		return _entities;
	}
	void update()
	{
		// remove entities flagged as inactive
		for (auto iter = _entities.begin(); iter != _entities.end();)
		{
			if (!(**iter).active)
			{
				// find and remove entity from caches
				list<vector<bool>> cacheKeys = _reverseIndex[*iter];
				for (auto keyIter = cacheKeys.begin(); keyIter != cacheKeys.end(); keyIter++)
				{
					_cache[*keyIter].remove(*iter);
				}
				_reverseIndex.erase(*iter);
				// remove entity from main list
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
			// add new entity to matching caches
			for (auto iter = _cache.begin(); iter != _cache.end(); iter++)
			{
				if (_newEntities.front()->has(iter->first))
				{
					iter->second.push_back(_newEntities.front());
					_reverseIndex[_newEntities.front()].push_back(iter->first);
				}
			}
			_newEntities.pop();
		}
	}
	void insert(Entity& entity)
	{
		_newEntities.push(shared_ptr<Entity>(&entity));
	}
	void remove(Entity& entity)
	{
		entity.active = false;
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