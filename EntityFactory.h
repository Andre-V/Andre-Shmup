#pragma once
#include <array>
#include "Entity.h"
#include "EntityInterfaces.h"
#include "ConcreteComponents.h"

class EntityManager;

class Something;
class Player;
class Enemy;
class Object;
class Bullet;
class Asteroid;
class GenericBullet;
class GameSpawner;

class EntityFactory
{
public:
	static SDL_Renderer* renderer;

	static Entity& make()
	{
		Entity* entity = new Entity;
		return *entity;
	}
	template<typename E, typename... ERest>
	static Entity& make(ERest...)
	{
		return this->make<E>();
	}

	template<>
	static Entity& make<AIShip>()
	{
		Entity& entity = make();
		entity.add<AIShip>();
		return entity;
	}
	template<>
	static Entity& make<Asteroid>()
	{
		Entity& entity = make<AIShip>();
		
		entity.add<Ship>();
		entity.add<Health>().health = 10;
		entity.add<Dimensions>();
		entity.add<Velocity>().velocity = float2{ 0, 10 };
		entity.add<TextureBox>().texture.renderer = renderer;

		entity.get<TextureBox>().texture.load("textures/test.png");
		entity.get<Dimensions>().w = 100;
		entity.get<Dimensions>().h = 100;

		return entity;
	}
	template<>
	static Entity& make<Asteroid,float2,float2>(float2 position, float2 velocity)
	{
		Entity& entity = make<Asteroid>();
		entity.add<Position>().position = position;
		entity.add<Velocity>().velocity = velocity;
		return entity;
	}

	template<>
	static Entity& make<Player>()
	{
		Entity& entity = make();

		entity.add<Player>();
		entity.add<Ship>();
		entity.add<Spawner>();
		entity.add<Position>().position = float2{ 384, 900 };
		entity.add<Velocity>().velocity = float2{ 0, 0 };
		entity.add<Dimensions>().w = 50;

		entity.get<Dimensions>().h = 50;

		entity.add<TextureBox>().texture.renderer = renderer;
		entity.get<TextureBox>().texture.load("textures/test.png");

		entity.get<Spawner>().active = false;
		entity.get<Spawner>().origin.reset(&entity.get<Position>().position);
		entity.get<Spawner>().ticks = 50;
		entity.get<Spawner>().add(&make<GenericBullet>(), 0);
		entity.get<Spawner>().add(&make<GenericBullet>(), 10);
		return entity;
	}
	template<>
	static Entity& make<Player,int,int>(int t, int t2)
	{

	}
	template<>
	static Entity& make<Enemy>()
	{

	}
	template<>
	static Entity& make<GenericBullet>()
	{
		Entity& entity = make();
		entity.active = false;
		entity.add<Bullet>().damage = 1;
		entity.add<Dimensions>().w = 20;
		entity.get<Dimensions>().h = 20;
		entity.add<Velocity>().velocity = float2{ 0, -5 };
		entity.add<TextureBox>().texture.renderer = renderer;
		entity.get<TextureBox>().texture.load("textures/test.png");
		return entity;
	}
	template<>
	static Entity& make<GameSpawner>()
	{
		Entity& entity = make();
		entity.add<Spawner>();
		entity.get<Spawner>().loop = true;
		return entity;
	}
};