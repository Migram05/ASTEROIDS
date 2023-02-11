#include "Component.h"
#include "../ecs/Manager.h"
#include "../ecs/Entity.h"
Component::Component() : ent_(), mngr_()
{

}

inline void Component::setContext(Entity* ent, Manager* mngr)
{
	ent_ = ent;
	mngr_ = mngr;
}

void Component::initComponent()
{
}

void Component::update()
{
}

void Component::render()
{
}
Component::~Component()
{
}
