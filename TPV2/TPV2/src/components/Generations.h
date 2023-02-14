#pragma once
#include "Component.h"
#include "Transform.h"
#include "../ecs/Entity.h"
class Generations : public Component
{
public:
	Generations();
	Generations(int n);
	inline int& GetGeneration() { return numGen; }
	void initComponent() override;

private:
	int numGen;
	Transform* tr_;
};

