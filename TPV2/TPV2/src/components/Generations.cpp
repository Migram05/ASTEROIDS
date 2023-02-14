#include "Generations.h"

Generations::Generations() : numGen(3)
{
}

Generations::Generations(int n) : numGen(n)
{
}

void Generations::initComponent()
{
	tr_ = ent_->getComponent<Transform>(ecs::_TRANSFORM);
	float& w = tr_->getW();
	float& h = tr_->getH();
	w = w * numGen; h = h * numGen;
}

