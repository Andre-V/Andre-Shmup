#include <iostream>
#include "Game.h"
#include "Component.h"

int main(int argc, char* args[])
{
	//Game game;

	//while (game.isRunning())
	//{
	//	game.update();
	//}
	std::cout << CompManager::getTypeID<Game>() << std::endl;
	std::cout << CompManager::getTypeID<Component>() << std::endl;
	std::cout << CompManager::getTypeID<Game>() << std::endl;
	std::cout << CompManager::getTypeID<int>() << std::endl;
	std::cout << CompManager::size() << std::endl;
	return 0;
}
