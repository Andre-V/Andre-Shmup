#pragma once
#include <array>
#include <math.h>
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
class Jet;
class BasicBullet;

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
		return make();
	}

	template<>
	static Entity& make<AIShip>()
	{
		Entity& entity = make();
		entity.add<AIShip>();
		entity.add<Ship>();
		entity.add<Health>();
		entity.add<Position>();
		entity.add<Velocity>();
		entity.add<Dimensions>();
		entity.add<TextureBox>().texture.renderer = renderer;
		return entity;
	}
	template<>
	static Entity& make<Asteroid>()
	{
		Entity& entity = make<AIShip>();
		
		entity.get<Health>().health = 10;
		entity.get<Velocity>().velocity = float2{ 0, 10 };
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
	static Entity& make<Jet>()
	{
		Entity& entity = make<AIShip>();
		entity.add<Jet>();
		entity.add<Spawner>();
		entity.get<AIShip>().stateOffset = 100;
		entity.get<Health>().health = 5;
		entity.get<TextureBox>().texture.load("textures/jet.png");
		entity.get<Dimensions>().w = 50;
		entity.get<Dimensions>().h = 50;
		entity.get<Velocity>().velocity = float2{ 0, 4 };
		entity.get<Spawner>().active = false;
		entity.get<Spawner>().loop = false;
		entity.get<Spawner>().add(&make<BasicBullet>(), 0);
		return entity;
	}
	template<>
	static Entity& make<Heli>()
	{
		Entity& entity = make<AIShip>();
		entity.add<Heli>();
		entity.add<Spawner>();
		entity.get<AIShip>().stateOffset = 100;
		entity.get<Health>().health = 10;
		entity.get<TextureBox>().texture.load("textures/heli.png");
		entity.get<Dimensions>().w = 75;
		entity.get<Dimensions>().h = 75;
		entity.get<Velocity>().velocity = float2{ 0, 3 };
		entity.get<Spawner>().add(&make<BasicBullet>(), 0);
		entity.get<Spawner>().add(&make<BasicBullet>(0.7854f), 0);
		entity.get<Spawner>().add(&make<BasicBullet>(-0.7854f), 0);
		entity.get<Spawner>().add(nullptr, 50);
		entity.get<Spawner>().add(&make<BasicBullet>(), 0);
		entity.get<Spawner>().add(&make<BasicBullet>(0.7854f), 0);
		entity.get<Spawner>().add(&make<BasicBullet>(-0.7854f), 0);
		entity.get<Spawner>().add(nullptr, 50);
		entity.get<Spawner>().add(&make<BasicBullet>(), 0);
		entity.get<Spawner>().add(&make<BasicBullet>(0.7854f), 0);
		entity.get<Spawner>().add(&make<BasicBullet>(-0.7854f), 0);
		entity.get<Spawner>().add(nullptr, 150);
		return entity;
	}

	template<>
	static Entity& make<Player>()
	{
		Entity& entity = make();

		entity.add<Player>();
		entity.add<Ship>();
		entity.add<Spawner>();
		entity.add<Position>().position = float2{ 0, 900 };
		entity.add<Velocity>().velocity = float2{ 0, 0 };
		entity.add<Dimensions>().w = 50;

		entity.get<Dimensions>().h = 50;

		entity.add<TextureBox>().texture.renderer = renderer;
		entity.get<TextureBox>().texture.load("textures/test.png");

		entity.get<Spawner>().active = false;
		entity.get<Spawner>().ticks = 0;
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
		entity.add<PlayersBullet>();
		return entity;
	}
	template<>
	static Entity& make<BasicBullet>()
	{
		Entity& entity = make();
		entity.add<Bullet>().damage = 1;
		entity.add<Dimensions>().w = 15;
		entity.get<Dimensions>().h = 15;
		entity.add<Velocity>().velocity = float2{ 0, 3 };
		entity.add<TextureBox>().texture.renderer = renderer;
		entity.get<TextureBox>().texture.load("textures/pellet.png");
		return entity;
	}
	template<>
	static Entity& make<BasicBullet, float>(float radians)
	{
		Entity& entity = make<BasicBullet>();
		entity.get<Velocity>().velocity = rot(radians,entity.get<Velocity>().velocity);
		return entity;
	}
	template<>
	static Entity& make<GameSpawner>()
	{
		Entity& entity = make();
		entity.add<Spawner>();
		entity.get<Spawner>().loop = false;
		return entity;
	}
	template<>
	static Entity& make<Background, string, float, float>(string path, float speed, float screenHeight)
	{
		Entity& entity = make();
		entity.add<Background>();
		entity.add<Position>();
		entity.add<Velocity>();
		entity.add<Dimensions>();
		entity.add<TextureBox>().texture.renderer = renderer;
		entity.get<TextureBox>().texture.load(path);
		entity.get<Velocity>().velocity = { 0, speed };

		// Adjust initial position and size
		Dimensions& dim = entity.get<Dimensions>();
		float2& pos = entity.get<Position>().position;
		int w, h;
		SDL_QueryTexture(entity.get<TextureBox>().texture.texture, nullptr, nullptr, &w, &h);
		dim.w = w; dim.h = h;
		pos.y -= dim.h / 2.0f - screenHeight;

		return entity;
	}
};