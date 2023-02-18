#include "Follow.h"

Follow::Follow(Entity* f, float s) : objective_(f), speed_(s)
{
}

void Follow::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
}

void Follow::update()
{
	Vector2D& newDir = tr_->getVel();
	Vector2D dir = objective_->getComponent<Transform>()->getPos() - tr_->getPos();
	dir = dir.normalize() * speed_;
	newDir = dir;
	//newDir.rotate(newDir.angle(objective_->getComponent<Transform>(ecs::_TRANSFORM)->getPos() - tr_->getPos()) > 0 ? 1.0f : -1.0f);
}

Follow::~Follow()
{
}
