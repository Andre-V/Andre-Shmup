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
	static std::map<string,Entity*> load(string filename, const GameInfo& gameInfo)
	{
		std::map<string, Entity*> dataMap;
		Entity& gameSpawner = EntityFactory::make<GameSpawner>();
		Spawner& spawner = gameSpawner.get<Spawner>();

		json j;
		ifstream file;

		file.open(filename);
		file >> j;

		// LOAD ENTITIES AND ADD TO SPAWNER
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
			if (offset >= 0)
			{
				aiShip.stateOffset = offset;
			}
			

			float2& position = entity->get<Position>().position;
			position = { posX, posY };
			if (posY == 0) { position -= entity->get<Dimensions>().h / 2; }
			if (posY == -1) { position.y = gameInfo.SCREEN_HEIGHT + entity->get<Dimensions>().h / 2; }
			/*
			else if (posY == 0) { position -= entity->get<Dimensions>().h / 2; }
			if (posX == 0) { position -= entity->get<Dimensions>().h / 2; }
			else if (posX == 0) { position -= entity->get<Dimensions>().h / 2; }
			*/

			spawner.add(entity, time);
		}
		// LOAD BACKGROUNDS
		Entity& background = EntityFactory::make<Background>(j["background"].get<string>(),0.3f, gameInfo.SCREEN_HEIGHT);
		// LOAD FOREGROUND
		Entity& foreground = EntityFactory::make<Background>(j["foreground"].get<string>(),0.7f,gameInfo.SCREEN_HEIGHT);


		file.close();

		dataMap["spawner"] = &gameSpawner;
		dataMap["background"] = &background;
		dataMap["foreground"] = &foreground;

		return dataMap;
	}
};