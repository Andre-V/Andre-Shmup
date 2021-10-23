#pragma once
#include "System.h"
#include "ConcreteComponents.h"
class SysPlayer : public System
{
public:
	void update() override { }
};

class SysRender : public System
{
public:
	void update() override 
	{
		list<shared_ptr<Entity>> entities = entitySource->get<TextureBox, Position, Dimensions>();
		for (auto& entity : entities)
		{
			float2& pos = entity->get<Position>().position;
			Dimensions& dim = entity->get<Dimensions>();
			entity->get<TextureBox>().texture.render(pos.x - (dim.w/2), pos.y - (dim.h/2), dim.w, dim.h);
		}
	}
};