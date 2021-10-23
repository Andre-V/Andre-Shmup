#pragma once
#include "Entity.h"

class Something;
class Player;

class EntityFactory
{
public:
	template<typename E>
	Entity& make()
	{
		return *(new Entity);
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