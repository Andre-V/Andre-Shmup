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
	static std::map<int, Entity& (*)()> typeMap;
	enum field { type, time, flip, offset, posX, posY};
public:
	static Entity& load(string filename)
	{
		Entity& gameSpawner = EntityFactory::make<GameSpawner>();
		Spawner& spawner = gameSpawner.get<Spawner>();
		spawner.loop = true;

		json j;
		ifstream file;

		file.open(filename);
		file >> j;

		for (auto e = j["entities"].begin(); e != j["entities"].end(); e++)
		{
			int type = (*e)[field::type].get<int>();

			Entity* entity;
			if (typeMap.count(type) == 0) { continue; }
			entity = &typeMap[type]();
			
			int time = (*e)[field::time].get<int>();
			bool flip = (*e)[field::flip].get<int>();
			int offset = (*e)[field::offset].get<int>();
			float posX = (*e)[field::posX].get<int>();
			float posY = (*e)[field::posY].get<int>();

			AIShip& aiShip = entity->get<AIShip>();
			aiShip.flipped = flip;
			aiShip.stateOffset = offset;

			entity->add<Position>().position = { posX, posY };

			spawner.add(entity, time);
		}

		file.close();
		return gameSpawner;
	}
};