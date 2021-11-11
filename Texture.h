#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <string>

class Texture
{
public:
	SDL_Texture* texture;
	SDL_Renderer* renderer;

	Texture() : texture(nullptr), renderer(nullptr) {}
	~Texture() { deallocate(); }

	Texture(SDL_Renderer* renderer, std::string path)
	{
		this->renderer = renderer;
		load(path);
	}

	bool load(std::string path)
	{
		deallocate();

		SDL_Texture* newTexture = nullptr;
		SDL_Surface* surface = IMG_Load(path.c_str());
		if (surface != nullptr)
		{
			SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGBA(surface->format, 0x00, 0x00, 0x00, 0xFF));
			newTexture = SDL_CreateTextureFromSurface(renderer, surface);
			SDL_FreeSurface(surface);
		}
		texture = newTexture;
		return texture != nullptr;
	}

	void deallocate()
	{
		if (texture != nullptr)
		{
			SDL_DestroyTexture(texture);
			texture = nullptr;
		}
	}

	void render(SDL_Rect& rect)
	{
		SDL_RenderCopy(renderer, texture, nullptr, &rect);
	}
	void render(SDL_Rect rect)
	{
		SDL_RenderCopy(renderer, texture, nullptr, &rect);
	}
	void render(Rect& rect)
	{
		this->render(rect.sdlRect());
	}
	void render(float x, float y, float w, float h)
	{
		this->render({ (int)x, (int)y, (int)w, (int)h });
	}
};