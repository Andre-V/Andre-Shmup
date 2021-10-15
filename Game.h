#pragma once

class EntityManager;
class SystemManager;

class Game
{
private:
	EntityManager* _entityManager = nullptr;
	SystemManager* _systemManager = nullptr;
public:
	void update();
	bool isRunning();
};