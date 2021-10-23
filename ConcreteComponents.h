#pragma once
#include <SDL.h>
#include "Component.h"
#include "linalg.h"
#include "Rect.h"
#include "Texture.h"

using namespace linalg;
using namespace aliases;

struct Health : public Component
{
	int health;
};
struct Position : public Component
{
	float2 position;
};
struct Velocity : public Component
{
	float2 velocity;
};
struct HitBox : public Component
{
	float w;
	float h;
};
struct TextureBox : public Component
{
	//Rect rect;
	Texture texture;
};