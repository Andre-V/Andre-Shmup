#pragma once
#include <SDL.h>
#include "EntityManager.h"
#include "SystemManager.h"
#include "GameInfo.h"

class EntityManager;
class SystemManager;



class Game
{
private:
	EntityManager _entityManager;
	SystemManager _systemManager;
	
	GameInfo _gameInfo;

	SDL_Window* _sdlWindow;
	SDL_Renderer* _sdlRenderer;
	SDL_Event _e;

	bool _running;
public:
	Game(const int width, const int height, const char* title = "")
		: _entityManager(), _systemManager(),
		_gameInfo(width, height, title), _e(), _running(true)
	{
		_sdlWindow = SDL_CreateWindow(_gameInfo.WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			_gameInfo.SCREEN_WIDTH, _gameInfo.SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

		_sdlRenderer = SDL_CreateRenderer(_sdlWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

		SDL_SetRenderDrawColor(_sdlRenderer, 0x00, 0x00, 0x00, 0x00);

		_gameInfo.sdlWindow = _sdlWindow;

		_gameInfo.sdlRenderer = _sdlRenderer;

		EntityFactory::renderer = _sdlRenderer;

	}
	void update()
	{
		while (SDL_PollEvent(&_e) != 0)
		{
			if (_e.type == SDL_QUIT) { _running = false; }
			else //if (_e.type == SDL_KEYDOWN)
			{
				_gameInfo.sdlEvents.push_back(_e);
			}
		}
		_entityManager.update();
		_systemManager.update();
		SDL_RenderPresent(_sdlRenderer);
		SDL_RenderClear(_sdlRenderer);
		_gameInfo.sdlEvents = {};
	}
	bool isRunning()
	{
		return _running;
	}
	void addSystem(System* system)
	{
		system->entitySource = &_entityManager;
		system->gameInfo = &_gameInfo;
		_systemManager.add(system);
	}
	EntityManager& enttMngr()
	{
		return _entityManager;
	}
	GameInfo& info()
	{
		return _gameInfo;
	}
};

