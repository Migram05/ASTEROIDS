#include "Entity.h"
#include "Manager.h"
Entity::Entity() : mngr_(nullptr), cmps_(), currCmps_(), alive_() //Constructora
{
	currCmps_.reserve(ecs::maxComponentId); //Reserva espacio
}

void Entity::setContext(Manager* mngr)
{
	mngr_ = mngr;
}
