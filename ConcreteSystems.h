#pragma once
#include <iostream>
#include <SDL_events.h>
#include "System.h"
#include "ConcreteComponents.h"

typedef list<shared_ptr<Entity>> Entities;

SDL_Rect toRect(const float2& position, const Dimensions& dimensions)
{
	return { (int)(position.x - (dimensions.w / 2)), 
		(int)(position.y - (dimensions.h / 2)), (int)dimensions.w, (int)dimensions.h };
}

class SysPlayerInput : public System
{
public:
	void update() override 
	{ 
		Entities entities = entitySource->get<Player>();
		for (auto& entity : entities)
		{
			float2& vel = entity->get<Velocity>().velocity;
			const Uint8* keystate = SDL_GetKeyboardState(nullptr);

			// TO DO: Use key-mapping to make this prettier
			if (!(keystate[SDL_SCANCODE_UP] || keystate[SDL_SCANCODE_DOWN]
				|| keystate[SDL_SCANCODE_W] || keystate[SDL_SCANCODE_S]))
			{
				vel.y = 0;
			}
			else
			{
				if (keystate[SDL_SCANCODE_UP] || keystate[SDL_SCANCODE_W]) { vel.y = -3; }
				if (keystate[SDL_SCANCODE_DOWN] || keystate[SDL_SCANCODE_S]) { vel.y = 3; }
			}
			if (!(keystate[SDL_SCANCODE_LEFT] || keystate[SDL_SCANCODE_RIGHT]
				|| keystate[SDL_SCANCODE_A] || keystate[SDL_SCANCODE_D]))
			{
				vel.x = 0;
			}
			else
			{
				if (keystate[SDL_SCANCODE_LEFT]
					|| keystate[SDL_SCANCODE_A]) { vel.x = -3; }
				if (keystate[SDL_SCANCODE_RIGHT]
					|| keystate[SDL_SCANCODE_D]) { vel.x = 3; }
			}
			if (keystate[SDL_SCANCODE_SPACE])
			{
				entity->get<Ship>().shoot = true;
			}
			else
			{
				entity->get<Ship>().shoot = false;
			}
			/*
			for (auto& input : gameInfo->sdlEvents)
			{
				switch (input.type)
				{
					case SDL_KEYDOWN:
						switch (input.key.keysym.sym)
						{
							case SDLK_UP:
								cout << "UP PRESSED" << endl;
								vel.y = -1;
								break;
							case SDLK_DOWN:
								cout << "DOWN PRESSED" << endl;
								vel.y = 1;
								break;
							case SDLK_LEFT:
								cout << "LEFT PRESSED" << endl;
								vel.x = -1;
								break;
							case SDLK_RIGHT:
								cout << "RIGHT PRESSED" << endl;
								vel.x = 1;
								break;
							default:
								break;
						}
						break;
					case SDL_KEYUP:
						switch (input.key.keysym.sym)
						{
							case SDLK_UP:
								cout << "UP RELEASED" << endl;
								vel.y = 0;
								break;
							case SDLK_DOWN:
								cout << "DOWN RELEASED" << endl;
								vel.y = 0;
								break;
							case SDLK_LEFT:
								cout << "LEFT RELEASED" << endl;
								vel.x = 0;
								break;
							case SDLK_RIGHT:
								cout << "RIGHT RELEASED" << endl;
								vel.x = 0;
								break;
							default:
								break;
						}
						break;
					default:
						break;
				}

			}*/
			
		}
	}
};

class SysJetAI : public System
{
	void update() override
	{
		Entities jets = entitySource->get<Jet>();
		for (auto& jet : jets)
		{
			AIShip& ai = jet->get<AIShip>();
			switch (ai.state)
			{
				case 0:
					ai.stateOffset -= 1;
					if (ai.stateOffset <= 0) 
					{ 
						ai.state = 1; 
					}
					break;
				case 1:
					jet->get<Ship>().shoot = true;
					jet->get<Velocity>().velocity = { 2, 2 };
					break;
				default:
					break;
			}
			if (ai.flipped)
			{
				jet->get<Velocity>().velocity.x *= -1;
			}
		}
	}
};

class SysMove : public System
{
public:
	void update() override
	{
		Entities entities = entitySource->get<Position, Velocity>();
		for (auto& entity : entities)
		{
			entity->get<Position>().position += entity->get<Velocity>().velocity;		
		}
	}
};

class SysRender : public System
{
public:
	void update() override 
	{
		Entities entities = entitySource->get<TextureBox, Position, Dimensions>();
		for (auto& entity : entities)
		{
			float2& pos = entity->get<Position>().position;
			Dimensions& dim = entity->get<Dimensions>();
			entity->get<TextureBox>().texture.render(toRect(pos, dim));
		}
	}
};

class SysShipShoot : public System
{
public:
	void update() override
	{
		
		Entities entities = entitySource->get<Ship, Spawner>();
		for (auto& entity : entities)
		{
			Spawner& spawner = entity->get<Spawner>();
			spawner.active = entity->get<Ship>().shoot;
		}
	}
};
class SysUpdateSpawners : public System
{
public:
	// Updates each spawners validEntites list.
	void update() override
	{
		Entities entities = entitySource->get<Spawner>();
		for (auto& entity : entities)
		{
			Spawner& spawner = entity->get<Spawner>();

			// Check if there's entities left to spawn
			if (spawner.index < spawner.sequence.size())
			{
				// Check if spawner is even active and
				if (spawner.active)
				{
					// Keep "deqeuing" until spawner ticks != front pair ticks
					pair<Entity*, int> next;
					while ((next = spawner.sequence[spawner.index]).second == spawner.ticks)
					{
						// Clone entity
						Entity* copy = new Entity(*next.first);
						copy->active = true;
						entitySource->insert(*copy);

						
						// Spawn entity relative to parent entity, (0,0) by default.
						if (!copy->has<Position>())
						{
							copy->add<Position>();
							if (entity->has<Position>())
							{
								copy->get<Position>().position = entity->get<Position>().position;
							}
						}

						// Update other spawner data
						spawner.ticks = 0;
						spawner.index++;
  						if (spawner.loop)
						{
							spawner.index %= spawner.sequence.size();
						}
						else
						{
							if (spawner.index >= spawner.sequence.size())
							{
								break;
							}
						}
					
					}
					spawner.ticks++;
				}
				// else, reset spawner
				else
				{
					spawner.index = 0;
					spawner.ticks = 0;
				}
			}
		}
	}
};

class SysHitEnemyCollisions : public System
{
	// TODO: Use hitboxes instead
	void update() override
	{
		Entities bullets = entitySource->get<Bullet, PlayersBullet, Position, Dimensions>();
		Entities enemies = entitySource->get<Ship, Health, Position, Dimensions>();
		for (auto& bullet : bullets)
		{
			for (auto& enemy : enemies)
			{
				float2& bPos = bullet->get<Position>().position;
				float2& ePos = enemy->get<Position>().position;
				Dimensions& bDim = bullet->get<Dimensions>();
				Dimensions& eDim = enemy->get<Dimensions>();

				SDL_Rect&& rect = toRect(bPos, bDim);
				SDL_Rect&& rect2 = toRect(ePos, eDim);

				// Box v Box collision
				if (SDL_HasIntersection(&rect,&rect2))
				{
					bullet->active = false;
					enemy->get<Health>().health -= bullet->get<Bullet>().damage;
				}
			}
		}
	}
};

class SysDestroyNoHealth : public System
{
	void update() override
	{
		Entities entities = entitySource->get<Health>();
		for (auto& entity : entities)
		{
			if (entity->get<Health>().health <= 0)
			{
				entity->active = false;
			}
		}
	}
};

class SysDestroyOutOfBounds : public System
{
	void update() override
	{
		Entities entities = entitySource->get<Bullet, Position>();
		for (auto& entity : entities)
		{
			if (entity->get<Position>().position.y < 0)
			{
				entity->active = false;
			}
		}
	}
};