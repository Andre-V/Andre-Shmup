#pragma once
#include <vector>
#include <memory>
#include "Component.h"

using namespace std;
class Component;

class Entity
{
private:
	vector<shared_ptr<Component>> _components;

public:
	bool active = true;

};