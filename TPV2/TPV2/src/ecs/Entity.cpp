#include "Entity.h"
#include "Manager.h"
Entity::Entity(bool v) : mngr_(nullptr), cmps_(), currCmps_(), alive_(), visible(v) //Constructora
{
	currCmps_.reserve(ecs::maxComponentId); //Reserva espacio
}

void Entity::setContext(Manager* mngr)
{
	mngr_ = mngr;
}
