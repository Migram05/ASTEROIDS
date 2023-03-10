#include "Component.h"
#include "../ecs/Manager.h"
#include "../ecs/Entity.h"
Component::Component() : ent_(), mngr_()
{

}


void Component::initComponent()
{
}

Component::~Component()
{
}
#ifdef COMPS
void Component::update()
{
}

void Component::render()
{
}
#endif // COMPS



