#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include "json.hpp"
#include "EntityFactory.h"

using namespace std;
using namespace nlohmann;

class EntityFileLoader
{
private:
	std::map<string, Entity& (*)()> stringToEntity;
public:
	static void load(string filename, IEntityInsert& manager)
	{
		json j;
		ifstream file;

		file.open(filename);

		file >> j;

		for (auto entity = j.begin(); entity != j.end(); entity++)
		{

		}

		file.close();
	}
};