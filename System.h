#pragma once
#include "EntityManager.h"
#include "GameInfo.h"

class System
{
public:
	EntityManager* entitySource;
	GameInfo* gameInfo;
	virtual void update() { }
};