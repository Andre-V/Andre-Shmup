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

	Spawner& spawner = factory.make<GameSpawner>().get<Spawner>();
	spawner.add(&factory.make<Asteroid>(float2{ 100,0 }, float2{ 0,2.1 }), 0);
	spawner.add(&factory.make<Asteroid>(float2{ 400,0 }, float2{ 0,2.3 }), 120);
	spawner.add(&factory.make<Asteroid>(float2{ 500,0 }, float2{ 0,2.2 }), 60);
	spawner.add(&factory.make<Asteroid>(float2{ 600,0 }, float2{ 0,1.7 }), 150);
	spawner.add(&factory.make<Asteroid>(float2{ 128,0 }, float2{ 0,2.5 }), 50);

	game.addSystem(new SysPlayerInput, 1);
	game.addSystem(new SysMove, 2);
	game.addSystem(new SysRender, 3);
	game.addSystem(new SysUpdateSpawners, 4);
	game.addSystem(new SysShipShoot, 5);
	game.addSystem(new SysBulletCollisions, 6);
	game.addSystem(new SysDestoryOutOfBounds, 7);
}


int main(int argc, char* args[])
{
	Entity* e = new Entity;
	Game game(768, 1024, "Shmup");
	init(game);
	while (game.isRunning())
	{
		game.update();
	}
	return 0;
}
