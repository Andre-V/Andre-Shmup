#pragma once
#include <SDL.h>
#include "EntityManager.h"
#include "SystemManager.h"

class EntityManager;
class SystemManager;

struct GameInfo
{
	const int SCREEN_WIDTH;
	const int SCREEN_HEIGHT;
	const char* WINDOW_TITLE;

	GameInfo(const int width, const int height, const char* title)
		: SCREEN_WIDTH(width), SCREEN_HEIGHT(height), WINDOW_TITLE(title)
	{

	}
};

class Game
{
private:
	EntityManager _entityManager;
	SystemManager _systemManager;
	EntityRequests _entitySource;
	
	GameInfo _gameInfo;

	SDL_Window* _sdlWindow;
	SDL_Renderer* _sdlRenderer;
public:
	Game(const int width, const int height, const char* title = "")
		: _entityManager(), _systemManager(), _entitySource(_entityManager),
		_gameInfo(width, height, title)
	{
		_sdlWindow = SDL_CreateWindow(_gameInfo.WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			_gameInfo.SCREEN_WIDTH, _gameInfo.SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		_sdlRenderer = SDL_CreateRenderer(_sdlWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		SDL_SetRenderDrawColor(_sdlRenderer, 0x00, 0x00, 0x00, 0x00);
	}
	void update()
	{
		_entityManager.update();
		_systemManager.update();
		SDL_RenderPresent(_sdlRenderer);
		SDL_RenderClear(_sdlRenderer);
	}
	bool isRunning()
	{
		return true;
	}
	void addSystem(System* system, int priority = 0)
	{
		system->entitySource = &_entitySource;
		_systemManager.addSystem(system, priority);
	}
	EntityRequests& getEntityRequests()
	{
		return _entitySource;
	}
};

