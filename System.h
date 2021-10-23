#pragma once
#include "IEntityRequests.h"

class System
{
public:
	EntityRequests* entitySource;
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