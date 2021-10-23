#pragma once
#include <set>
#include <memory>
#include "System.h"

using namespace std;

class SystemManager
{
public:
	multiset<unique_ptr<System>,SystemCompare> systems;
	void addSystem(System* system, int priority = 0)
	{
		system->priority = priority;
		systems.insert(unique_ptr<System>(system));
	}
	void update()
	{
		for (auto iter = systems.begin(); iter != systems.end(); iter++)
		{
			(**iter).update();
		}
	}
};