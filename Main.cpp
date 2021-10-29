#include <iostream>
#include <vector>
#include <string>
#include "Game.h"
#include "Component.h"
#include "Entity.h"
#include "ConcreteSystems.h"

void init(Game& game)
{

	EntityFactory& factory = game.enttMngr().entityFactory;
	factory.make<Player>();

	game.addSystem(new SysPlayerInput, 1);
	game.addSystem(new SysMove, 2);
	game.addSystem(new SysRender, 3);
	game.addSystem(new SysUpdateSpawners, 4);
	game.addSystem(new SysShipShoot, 5);
}


int main(int argc, char* args[])
{
	Entity* e = new Entity;
	Game game(1280, 720, "Shmup");
	init(game);
	while (game.isRunning())
	{
		game.update();
	}
	return 0;
}
