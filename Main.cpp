#include <iostream>
#include <vector>
#include <string>
#include "Game.h"
#include "Component.h"
#include "Entity.h"
#include "ConcreteSystems.h"
#include "FileLoadFactory.h"

void init(Game& game)
{

	EntityFactory factory;
	
	std::map<string,Entity*> gameData = EntityFileLoader::load("levels/level_0", game);
	
	game.enttMngr().insert(*gameData["spawner"]);
	game.enttMngr().insert(*gameData["background"]);
	game.enttMngr().insert(*gameData["foreground"]);
	game.enttMngr().insert(factory.make<Player>());

	game.addSystem(new SysPlayerInput);
	game.addSystem(new SysJetAI);
	game.addSystem(new SysHeliAI);
	game.addSystem(new SysStealthAI);
	game.addSystem(new SysMove);
	game.addSystem(new SysMoveCamera);
	game.addSystem(new SysRenderBackground);
	game.addSystem(new SysRender);
	game.addSystem(new SysUpdateSpawners);
	game.addSystem(new SysShipShoot);
	game.addSystem(new SysHitEnemyCollisions);
	game.addSystem(new SysPlayerCollisions);
	game.addSystem(new SysDestroyNoHealth);
	game.addSystem(new SysDestroyOutOfBounds);
	
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
