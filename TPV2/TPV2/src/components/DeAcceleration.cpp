#include "DeAcceleration.h"
#include "../ecs/Manager.h"
DeAcceleration::DeAcceleration() : reduction(0.005) //Constructora por defecto
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
	Vector2D& f = tr_->getLastForward(); //Se obtiene el ultimo vector forward de la nave
	if (abs(v.getX()) > stopMargin || abs(v.getY()) > stopMargin) //En caso de que la velocidad sea superior al margen se reduce
		v = v + Vector2D{ f.getX() * -reduction, f.getY() * reduction };
	else v = { 0,0 }; //En caso de ser menor al margen, se detiene la nave
}
#endif // COMPS



