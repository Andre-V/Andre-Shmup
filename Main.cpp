#include <iostream>
#include <vector>
#include <string>
#include "Game.h"
#include "Component.h"
#include "Entity.h"
#include "ConcreteSystems.h"

void init(Game& game)
{
	Entity& entity = game.enttMngr().make<Player>();
	entity.add<Player>();
	entity.add<Position>().position = float2{ 500, 500 };
	entity.add<Velocity>().velocity = float2{ 0, 0 };
	entity.add<Dimensions>().w = 50;
	entity.get<Dimensions>().h = 50;
	entity.add<TextureBox>().texture.renderer = game.info().sdlRenderer;
	entity.get<TextureBox>().texture.load("textures/test.png");
	
	game.addSystem(new SysPlayerInput, 1);
	game.addSystem(new SysMove, 2);
	game.addSystem(new SysRender, 3);
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
