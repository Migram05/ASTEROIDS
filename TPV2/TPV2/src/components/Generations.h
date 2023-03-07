#pragma once
#include "Component.h"
#include "Transform.h"
#include "../ecs/Entity.h"
struct Generations : public Component
{
public:
	Generations();
	Generations(int n);
	inline int& GetGeneration() { return numGen; }
	void initComponent() override;
	constexpr static cmpId_type id = ecs::_GENERATIONS;
private:
	int numGen;
	Transform* tr_;
};

