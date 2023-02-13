#include "DisableOnExit.h"

DisableOnExit::DisableOnExit(int w, int h) : winWidth_(w), winHeight_(h)
{
}

DisableOnExit::~DisableOnExit()
{
}

void DisableOnExit::initComponent()
{
	tr_ = ent_->getComponent<Transform>(ecs::_TRANSFORM);
}

void DisableOnExit::update()
{
	Vector2D& position = tr_->getPos();
	if (position.getX() + tr_->getW() < 0 || position.getX() > winWidth_ || position.getY() + tr_->getH() < 0 || position.getY() > winHeight_) ent_->setAlive(false);
}
