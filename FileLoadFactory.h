#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include "json.hpp"
#include "EntityFactory.h"
#include "GameInfo.h"

using namespace std;
using namespace nlohmann;

class EntityFileLoader
{
private:
	static std::map<int, Entity& (*)()> typeMap;
	enum field { type, time, flip, offset, posX, posY};
public:
	static Entity& load(string filename, const GameInfo& gameInfo)
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
			
			int time = (*e)[field::time].get<float>()*75;
			bool flip = (*e)[field::flip].get<int>();
			int offset = (*e)[field::offset].get<float>()*75;
			float posX = (*e)[field::posX].get<int>();
			float posY = (*e)[field::posY].get<int>();

			AIShip& aiShip = entity->get<AIShip>();
			aiShip.flipped = flip;
			aiShip.stateOffset = offset;

			float2& position = entity->get<Position>().position;
			position = { posX, posY };
			if (posY == 0) { position -= entity->get<Dimensions>().h / 2; }
			/*
			else if (posY == 0) { position -= entity->get<Dimensions>().h / 2; }
			if (posX == 0) { position -= entity->get<Dimensions>().h / 2; }
			else if (posX == 0) { position -= entity->get<Dimensions>().h / 2; }
			*/

			spawner.add(entity, time);
		}

		file.close();
		return gameSpawner;
	}
};