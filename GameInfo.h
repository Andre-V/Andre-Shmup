#pragma once
#include <SDL_render.h>
#include <SDL_events.h>
#include <vector>

using namespace std;

struct GameInfo
{
	const int SCREEN_WIDTH;
	const int SCREEN_HEIGHT;
	const char* WINDOW_TITLE;
	SDL_Window* sdlWindow;
	SDL_Renderer* sdlRenderer;
	vector<SDL_Event> sdlEvents;

	GameInfo(const int width, const int height, const char* title)
		: SCREEN_WIDTH(width), SCREEN_HEIGHT(height), WINDOW_TITLE(title),
		sdlWindow(nullptr), sdlRenderer(nullptr)
	{

	}
};