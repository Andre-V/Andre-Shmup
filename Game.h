#pragma once
#include <SDL.h>
#include "EntityManager.h"
#include "SystemManager.h"
#include "GameInfo.h"

class EntityManager;
class SystemManager;



class Game : public GameInfo
{
private:
	EntityManager _entityManager;
	SystemManager _systemManager;
	SDL_Event _e;
	bool _running;
public:
	Game(const int width, const int height, const char* title = "")
		: GameInfo(width, height, title),
		_entityManager(), _systemManager(), _e(), _running(true)
	{
		sdlWindow = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

		sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

		SDL_SetRenderDrawColor(sdlRenderer, 0x00, 0x00, 0x00, 0x00);

		EntityFactory::renderer = sdlRenderer;

	}
	void update()
	{
		while (SDL_PollEvent(&_e) != 0)
		{
			if (_e.type == SDL_QUIT) { _running = false; }
			else
			{
				sdlEvents.push_back(_e);
			}
		}
		_entityManager.update();
		_systemManager.update();
		SDL_RenderPresent(sdlRenderer);
		SDL_RenderClear(sdlRenderer);
		sdlEvents.clear();
	}
	bool isRunning()
	{
		return _running;
	}
	void addSystem(System* system)
	{
		system->entitySource = &_entityManager;
		system->gameInfo = this;
		_systemManager.add(system);
	}
	EntityManager& enttMngr()
	{
		return _entityManager;
	}
	GameInfo& info()
	{
		return *this;
	}
};

