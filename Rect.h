#pragma once
#include <SDL_rect.h>

struct Rect
{
	float x = 0, y = 0, w = 0, h = 0;
	float centerX() { return x + (w / 2); }
	float centreY() { return y + (h / 2); }
	void centreX(float x) { this->x = x - (w / 2); }
	void centreY(float y) { this->y = y - (h / 2); }
	float bottomX() { return x + w; }
	float bottomY() { return y + h; }
	SDL_Rect sdlRect()
	{
		return { (int)x, (int)y, (int)w, (int)h };
	}
};