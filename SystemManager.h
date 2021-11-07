#pragma once
#include <set>
#include <memory>
#include "System.h"

using namespace std;

class SystemManager
{
public:
	vector<unique_ptr<System>> systems;
	void add(System* system)
	{
		systems.push_back(unique_ptr<System>(system));
	}
	void update()
	{
		for (auto iter = systems.begin(); iter != systems.end(); iter++)
		{
			(**iter).update();
		}
	}
};