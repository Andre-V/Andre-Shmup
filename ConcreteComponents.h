#pragma once
#include <SDL.h>
#include "Component.h"
#include "linalg.h"
#include "Rect.h"
#include "Texture.h"

using namespace linalg;
using namespace aliases;

// Class components: components that identify a kind of entity
struct Ship : public Component
{
	bool shoot = false;
	Ship* clone() { return new Ship(*this); }
};
struct Player : public Component { };
// Data components: components that hold data
struct Health : public Component
{
	int health = 0;
	Health* clone() { return new Health(*this); }
};
struct Position : public Component
{
	float2 position;
	Position* clone() { return new Position(*this); }
};
struct Velocity : public Component
{
	float2 velocity;
	Velocity* clone() { return new Velocity(*this); }
};
struct Dimensions : public Component
{
	float w = 0, h = 0;
	Dimensions* clone() { return new Dimensions(*this); }
};
struct TextureBox : public Component
{
	//float w, h;
	Texture texture;
	TextureBox* clone() { return new TextureBox(*this); }
};
struct Spawner : public Component
{
	queue<pair<Entity*, int>> sequence;
	float2* origin = nullptr;
	int ticks = 0;
	bool active = true;
	bool loop = true;
	list<Entity*> validEntities;
	Spawner* clone() { return new Spawner(*this); }
};