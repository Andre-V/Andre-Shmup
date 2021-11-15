#include "FileLoadFactory.h"
std::map<int, Entity& (*)()> EntityFileLoader::typeMap = { 
	{ 0, EntityFactory::make<Jet> }, 
	{ 1, EntityFactory::make<Heli> },
	{ 2, EntityFactory::make<Stealth> }
};