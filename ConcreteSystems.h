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
		list<shared_ptr<Entity>> entities = entitySource->get<TextureBox, Position>();
		for (auto& entity : entities)
		{
			float2& pos = entity->get<Position>().position;
			Rect rect = { (int)pos.x, (int)pos.y, 50, 50 };
			entity->get<TextureBox>().texture.render(rect);
		}
	}
};