#include "DeAcceleration.h"

DeAcceleration::DeAcceleration() : reduction(0.005) //Constructora por defecto
{
}

DeAcceleration::DeAcceleration(float r) : reduction(r) //Constructora por valor
{
}


void DeAcceleration::initComponent() //Se inicializa
{
	tr_ = ent_->getComponent<Transform>(); //Obtiene el transform
}

void DeAcceleration::update()
{
	Vector2D& v = tr_->getVel(); //Se obtiene la velocidad
	Vector2D& f = tr_->getLastForward(); //Se obtiene el ultimo vector forward de la nave
	if (abs(v.getX()) > stopMargin || abs(v.getY()) > stopMargin) //En caso de que la velocidad sea superior al margen se reduce
		v = v + Vector2D{ f.getX() * -reduction, f.getY() * reduction };
	else v = { 0,0 }; //En caso de ser menor al margen, se detiene la nave
}

DeAcceleration::~DeAcceleration() //Destructora
{
}