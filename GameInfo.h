#pragma once
#include <SDL_render.h>
#include <SDL_events.h>
#include <vector>

using namespace std;

struct GameInfo
{
	const float SCREEN_WIDTH;
	const float SCREEN_HEIGHT;
	const float WIDTH_HALF;
	const float MAX_PLAY_DISTANCE;
	const char* WINDOW_TITLE;
	SDL_Window* sdlWindow;
	SDL_Renderer* sdlRenderer;
	vector<SDL_Event> sdlEvents;
	float2 cameraPosition;

	GameInfo(const int width, const int height, const char* title)
		: SCREEN_WIDTH(width), SCREEN_HEIGHT(height), WINDOW_TITLE(title),
		sdlWindow(nullptr), sdlRenderer(nullptr),
		cameraPosition({ 0.0f - SCREEN_WIDTH / 2.0f,0 }),
		WIDTH_HALF(SCREEN_WIDTH / 2.0f),
		MAX_PLAY_DISTANCE(WIDTH_HALF + 100.0f)
	{

	}
};