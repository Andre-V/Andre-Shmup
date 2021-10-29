#pragma once
#include "EntityManager.h"
#include "GameInfo.h"

class System
{
public:
	EntityManager* entitySource;
	GameInfo* gameInfo;
	int priority;
	virtual void update() { }
};

struct SystemCompare
{
	bool operator()(const unique_ptr<System>& lhs, const unique_ptr<System>& rhs) const
	{
		return lhs->priority < rhs->priority;
	}
};

