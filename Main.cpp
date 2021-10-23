#include <iostream>
#include <vector>
#include "Game.h"
#include "Component.h"
#include "Entity.h"

int main(int argc, char* args[])
{
	Entity* e = new Entity;
	Game game(1280, 720, "Shmup");
	while (game.isRunning())
	{
		game.update();
	}
	return 0;
}
