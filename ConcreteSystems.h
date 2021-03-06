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
			float2& pos = entity->get<Position>().position;
			Dimensions& dim = entity->get<Dimensions>();
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
					|| keystate[SDL_SCANCODE_A]) { vel.x = -4; }
				if (keystate[SDL_SCANCODE_RIGHT]
					|| keystate[SDL_SCANCODE_D]) { vel.x = 4; }
			}
			if (keystate[SDL_SCANCODE_SPACE])
			{
				entity->get<Ship>().shoot = true;
			}
			else
			{
				entity->get<Ship>().shoot = false;
			}

			if (pos.y - dim.h / 2 <= 0 && vel.y < 0)
			{
				vel.y = 0;
			}
			else if (pos.y + dim.h / 2 >= gameInfo->SCREEN_HEIGHT && vel.y > 0)
			{
				vel.y = 0;
			}
			if (pos.x - dim.w / 2 <= -gameInfo->MAX_PLAY_DISTANCE && vel.x < 0)
			{
				vel.x = 0;
			}
			else if (pos.x + dim.w / 2 >= gameInfo->MAX_PLAY_DISTANCE && vel.x > 0)
			{
				vel.x = 0;
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
						jet->get<Ship>().shoot = true;
						jet->get<Velocity>().velocity = { 2, 2 };
						if (ai.flipped)
						{
							jet->get<Velocity>().velocity.x *= -1;
						}
					}
					break;
				case 1:
					break;
				default:
					break;
			}
		}
	}
};

class SysHeliAI : public System
{
	void update() override
	{
		Entities helis = entitySource->get<Heli>();
		for (auto& heli : helis)
		{
			AIShip& ai = heli->get<AIShip>();
			ai.stateOffset -= 1;
			switch (ai.state)
			{
				case 0: // MOVE FORWARD PHASE
					if (ai.stateOffset <= 0)
					{
						ai.stateOffset = 250;
						ai.state = 1;
						heli->get<Velocity>().velocity = { 0, 0.3 };
						heli->get<Ship>().shoot = true;
					}
					break;
				case 1: // STATIONARY SHOOT PHASE
					if (ai.stateOffset <= 0)
					{
						ai.stateOffset = 60;
						ai.state = 2;
						heli->get<Velocity>().velocity = { 2, 0 };
						heli->get<Ship>().shoot = false;
						if (ai.flipped)
						{
							heli->get<Velocity>().velocity.x *= -1;
						}
					}
					break;
				case 2: // MOVE SIDEWAYS PHASE
					if (ai.stateOffset <= 0)
					{
						ai.stateOffset = 250;
						ai.state = 1;
						ai.flipped = !ai.flipped;
						heli->get<Velocity>().velocity = { 0, 0.3 };
						heli->get<Ship>().shoot = true;
					}
					break;
				default:
					break;
			}
			
		}
	}
};

class SysStealthAI : public System
{
	void update() override
	{
		Entities stealths = entitySource->get<Stealth>();
		Entities players = entitySource->get<Player>();
		shared_ptr<Entity> player = nullptr;
		if (players.size() > 0)
		{
			player = *(players.begin());
		}
		else
		{
			return;
		}

		for (auto& stealth : stealths)
		{
			AIShip& ai = stealth->get<AIShip>();
			float2& pos = stealth->get<Position>().position;
			float2& vel = stealth->get<Velocity>().velocity;
			ai.stateOffset -= 1;
			float diff = pos.x - player->get<Position>().position.x;
			switch (ai.state)
			{
				case 0: // MOVE FORWARD
					if (ai.stateOffset <= 0)
					{
						ai.state = 1;
						vel.y = -1;
					}
					break;
				case 1: // MOVE TOWARDS PLAYER
					if (player != nullptr)
					{
						if (diff > 0)
						{
							vel.x = -3;
						}
						else
						{
							vel.x = 3;
						}
						if (diff < 100 && diff > -100) // STATE 2 IF WITHIN RANGE
						{
							stealth->get<Ship>().shoot = true;
							ai.state = 2;
							vel.y = 0.1;
						}
					}
					break;
				case 2: // SHOOT + SLOWLY FOLLOW PLAYER
					if (player != nullptr)
					{
						if (diff > 0)
						{
							vel.x = -1;
						}
						else
						{
							vel.x = 1;
						}
						if (diff > 200 || diff < -200)
						{
							stealth->get<Ship>().shoot = false;
							ai.state = 1;
							vel.y = -0.5;
						}
					}
					break;
				default:
					break;
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

class SysMoveCamera: public System
{
public:
	// Move camera according to player position
	void update() override
	{
		Entities entities = entitySource->get<Player>();
		for (auto& entity : entities)
		{
			float&& prop = entity->get<Position>().position.x / gameInfo->MAX_PLAY_DISTANCE;
			gameInfo->cameraPosition.x = (gameInfo->MAX_PLAY_DISTANCE - gameInfo->WIDTH_HALF)*prop - gameInfo->WIDTH_HALF;
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
			entity->get<TextureBox>().texture.render(toRect(pos - gameInfo->cameraPosition, dim));
		}
	}
};

class SysRenderBackground : public System
{
	void update() override
	{
		Entities entities = entitySource->get<Background, Position, Dimensions, TextureBox>();
		for (auto& entity : entities)
		{
			float2& pos = entity->get<Position>().position;
			Dimensions& dim = entity->get<Dimensions>();
			// Detect background going off-screen to reset
			float cornerY = pos.y - dim.h / 2;
			if (cornerY >= gameInfo->SCREEN_HEIGHT)
			{
				pos.y = 0;
				pos.y -= dim.h / 2.0f - gameInfo->SCREEN_HEIGHT;
			}
			// Render duplicate
			else if (cornerY > 0)
			{
				entity->get<TextureBox>().texture.render(
					toRect(pos - float2{0,dim.h} - gameInfo->cameraPosition, dim)
);
			}
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
						// Skip null entities, can also be used to add delays
						if (next.first != nullptr)
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
class SysPlayerCollisions : public System
{
	void update() override
	{
		Entities players = entitySource->get<Player>();
		Entities entities = entitySource->get<Bullet, EnemysBullet, Position>();
		entities.splice(entities.begin(), entitySource->get<AIShip, Position>());
		for (auto& player : players)
		{
			for (auto& entity : entities)
			{
				SDL_Rect&& pRect = toRect(
					player->get<Position>().position, 
					player->get<Dimensions>()
				);
				SDL_Rect&& eRect = toRect(
					entity->get<Position>().position,
					entity->get<Dimensions>()
				);
				if (SDL_HasIntersection(&pRect, &eRect))
				{
					player->active = false;
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
		const int buffer = 200;
		Entities entities = entitySource->get<Bullet, Position>();
		entities.splice(entities.begin(), entitySource->get<Ship, Position>());
		
		int x, y;
		for (auto& entity : entities)
		{
			x = entity->get<Position>().position.x;
			y = entity->get<Position>().position.y;
			if (y + buffer < 0 || y - buffer > gameInfo->SCREEN_HEIGHT)
			{
				entity->active = false;
			}
			else if (x + buffer < 0 - gameInfo->MAX_PLAY_DISTANCE || x - buffer > gameInfo->MAX_PLAY_DISTANCE)
			{
				entity->active = false;
			}
		}
	}
};