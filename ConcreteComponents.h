#pragma once
#include <SDL.h>
#include "Component.h"
#include "linalg.h"
#include "Rect.h"
#include "Texture.h"

using namespace linalg;
using namespace aliases;

// Class components: components that identify a kind of entity
struct Player : public Component { };
// Data components: components that hold data
struct Health : public Component
{
	int health = 0;
};
struct Position : public Component
{
	float2 position;
};
struct Velocity : public Component
{
	float2 velocity;
};
struct Dimensions : public Component
{
	float w = 0, h = 0;
};
struct TextureBox : public Component
{
	//float w, h;
	Texture texture;
};
struct Spawner : public Component
{
	float2* origin;
};