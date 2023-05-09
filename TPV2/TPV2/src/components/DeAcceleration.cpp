#include "DeAcceleration.h"
#include "../ecs/Manager.h"
DeAcceleration::DeAcceleration() //Constructora por defecto
{
}

DeAcceleration::DeAcceleration(float r) : reduction(r) //Constructora por valor
{
}


void DeAcceleration::initComponent() //Se inicializa
{
	tr_ = mngr_->getComponent<Transform>(ent_); //Obtiene el transform
}

DeAcceleration::~DeAcceleration() //Destructora
{
}

#ifdef COMPS
void DeAcceleration::update()
{
	Vector2D& v = tr_->getVel(); //Se obtiene la velocidad
	v = v * reduction;
	if (v.magnitude() <= stopMargin) v = { 0,0 };
}
#endif // COMPS



