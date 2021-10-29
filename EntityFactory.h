#pragma once
#include "Entity.h"
#include "EntityInterfaces.h"
#include "ConcreteComponents.h"

class EntityManager;

class Something;
class Player;
class Enemy;
class Bullet;
class GenericBullet;

class EntityFactory
{
private:
	// To expose the EntityFactory's templates, I had to allow use
	// of EntityFactory directly.
	IEntityInsert& _entityInserter;
	SDL_Renderer* _renderer;

public:
	EntityFactory(IEntityInsert& entityManager)
		: _entityInserter(entityManager)
	{
		
	}
	void setRenderer(SDL_Renderer* renderer)
	{
		_renderer = renderer;
	}
	Entity& make(bool insert = true)
	{
		Entity* entity = new Entity;
		_entityInserter.insert(*entity);
		return *entity;
	}
	template<typename E>
	Entity& make()
	{
		return make();
	}
	template<typename E, typename... ERest>
	Entity& make(ERest...)
	{
		return this->make<E>();
	}

	template<>
	Entity& make<Player>()
	{
		Entity& entity = make();

		entity.add<Player>();
		entity.add<Ship>();
		entity.add<Spawner>();

		entity.add<Position>().position = float2{ 500, 500 };
		entity.add<Velocity>().velocity = float2{ 0, 0 };
		entity.add<Dimensions>().w = 50;
		entity.get<Dimensions>().h = 50;
		entity.add<TextureBox>().texture.renderer = _renderer;
		entity.get<TextureBox>().texture.load("textures/test.png");
		entity.get<Spawner>().origin = &entity.get<Position>().position;
		entity.get<Spawner>().ticks = 50;
		entity.get<Spawner>().sequence.push({ &make<GenericBullet>(), 0 });
		entity.get<Spawner>().sequence.push({ &make<GenericBullet>(), 10 });
		return entity;
	}
	template<>
	Entity& make<Player,int,int>(int t, int t2)
	{

	}
	template<>
	Entity& make<Enemy>()
	{

	}
	template<>
	Entity& make<GenericBullet>()
	{
		Entity& entity = make();
		entity.add<Dimensions>().w = 20;
		entity.get<Dimensions>().h = 20;
		entity.add<Velocity>().velocity = float2{ 0, -5 };
		entity.add<TextureBox>().texture.renderer = _renderer;
		entity.get<TextureBox>().texture.load("textures/test.png");
		return entity;
	}

};