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
struct AIShip : public Component 
{
	int state = 0;
	bool flipped = false;
	int stateOffset = 100;
	AIShip* clone() { return new AIShip(*this); }
};
struct Jet : public Component { };
struct Heli : public Component { };
struct Stealth : public Component { };
struct Bullet : public Component 
{ 
	float damage = 0;
	Bullet* clone() { return new Bullet(*this); }
};
struct EnemysBullet : public Component { };
struct PlayersBullet : public Component {};
// Data components: components that hold data
struct Health : public Component
{
	float health = 1;
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
struct Animation : public Component
{
	SDL_Rect frameRect = { 0,0,0,0 };
	int frameIndex = 0;
	int maxFrames = 0;
	int frameTime = 0;
	bool loop = false;
};
struct Spawner : public Component
{
	vector<pair<Entity*, int>> sequence;
	vector<float2> offsets;
	int index = 0;
	int ticks = 0;
	bool active = true;
	bool loop = true;

	void add(Entity* entity, int ticks)
	{
		if (entity != nullptr) { entity->active = false; }
		sequence.push_back({ entity, ticks });
	}
	void add(Entity* entity, int ticks, float2 offset)
	{
		this->add(entity, ticks);
		offsets.push_back(offset);
	}
	Spawner* clone()  {  return new Spawner(*this); }
};
struct Background : public Component
{

};