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
	float mag = sqrt((dir.getX() * dir.getX()) + (dir.getY() * dir.getY()));
	dir = Vector2D((dir.getX() / mag) / 10, (dir.getY() / mag) / 10);
	newDir = dir;
}

Follow::~Follow()
{
}
