#include "Follow.h"

Follow::Follow(Entity* f) : objective_(f)
{
}

void Follow::initComponent()
{
	tr_ = ent_->getComponent<Transform>(ecs::_TRANSFORM);
}

void Follow::update()
{
	Vector2D& newDir = tr_->getVel();
	Vector2D dir = objective_->getComponent<Transform>(ecs::_TRANSFORM)->getPos() - tr_->getPos();
	dir = dir.normalize() / 10;
	newDir = dir;
}

Follow::~Follow()
{
}
