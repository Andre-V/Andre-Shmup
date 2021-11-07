#pragma once
#include <vector>
#include <memory>
#include "Component.h"

using namespace std;
class Component;

class Entity
{
private:
	vector<shared_ptr<Component>> _components;
	vector<bool> _key;
	void resize(size_t newSize)
	{
		_components.resize(newSize);
		_key.resize(newSize);
	}
	void checkCapacity(size_t index)
	{
		if (index >= _components.size())
		{
			resize(index + 1);
		}
	}
public:
	bool active; bool exists;

	Entity() : 
		_components(CmpMngr::size()),
		_key(CmpMngr::size()),
		active(true), exists(true)
	{
		
	}
	
	Entity(const Entity& entity) 
		: _components(entity._components.size()), _key(entity._key), active(entity.active), exists(entity.exists)
	{
		for (int i = 0; i < entity._components.size(); i++)
		{
			if (entity._components[i] != nullptr)
			{
				_components[i] = shared_ptr<Component>(entity._components[i]->clone());
			}
			
		}
	}
	
	template<typename T, typename T2, typename... TRest> 
	bool has()
	{
		int&& index = CmpMngr::getTypeID<T>();
		checkCapacity(index);
		return _key[index] ? _key[index] && has<T2, TRest...>() : false;
	}
	template<typename T> 
	bool has()
	{
		int&& index = CmpMngr::getTypeID<T>();
		checkCapacity(index);
		return _key[CmpMngr::getTypeID<T>()];
	}
	// adds new component
	template<typename T>
	T& add()
	{
		T* c(new T);
		shared_ptr<Component> compPtr(c);
		
		int&& index = CmpMngr::getTypeID<T>();

		checkCapacity(index);

		_components[index] = compPtr;
		_key[index] = true;

		return *c;
	}

	template<typename T>
	T& get()
	{
		return (T&)(*_components[CmpMngr::getTypeID<T>()]);
	}
};