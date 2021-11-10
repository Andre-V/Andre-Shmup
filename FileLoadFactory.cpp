#include "FileLoadFactory.h"
std::map<int, Entity& (*)()> EntityFileLoader::typeMap = { 
	{ 0, EntityFactory::make<Jet> } 
};