#include "DeAcceleration.h"

DeAcceleration::DeAcceleration() : reduction(0.0005)
{
}

DeAcceleration::DeAcceleration(float r) : reduction(r)
{
}

DeAcceleration::~DeAcceleration()
{
}

void DeAcceleration::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
}

void DeAcceleration::update()
{
	Vector2D& v = tr_->getVel();
	Vector2D& f = tr_->getLastForward();
	if (abs(v.getX()) > stopMargin || abs(v.getY()) > stopMargin)
		v = v + Vector2D{ f.getX() * -reduction, f.getY() * reduction };
	else v = { 0,0 };
}
