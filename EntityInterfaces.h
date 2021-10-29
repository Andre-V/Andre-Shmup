#pragma once
#include "Entity.h"
class IEntityInsert
{
public:
	virtual void insert(Entity& entity) = 0;
};