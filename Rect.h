#pragma once
#include <SDL_rect.h>

struct Rect
{
	float x, y, w, h;
	float centerX() { return x + (w / 2); }
	float centreY() { return y + (h / 2); }
	void centreX(float x) { this->x = x - (w / 2); }
	void centreY(float y) { this->y = y - (h / 2); }
	float bottomX() { return x + w; }
	float bottomY() { return y + h; }
	SDL_Rect sdlRect()
	{
		return { x, y, w, h };
	}
};