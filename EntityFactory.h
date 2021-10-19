#pragma once
#include "Entity.h"

class Something;

class EntityFactory
{
public:
	template<typename E>
	Entity& make()
	{
		return *(new Entity);
	}
	
	template<>
	Entity& make<Something>()
	{

	}
};