#pragma once
#include "Component.h"
class Generations : public Component
{
public:
	Generations();
	Generations(int n);
	inline int& GetGeneration() { return numGen; }

private:
	int numGen;
};

