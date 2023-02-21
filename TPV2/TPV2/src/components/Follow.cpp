#include "Follow.h"

Follow::Follow(Entity* f, float s) : objective_(f), speed_(s) //Constructora, se guarda al objetivo a seguir
{
}

void Follow::initComponent()
{
	tr_ = ent_->getComponent<Transform>(); //Inicializa el componente buscando el transform
}

void Follow::update()
{
	Vector2D& newDir = tr_->getVel();
	//Calcula el nuevo vector dirección
	Vector2D dir = objective_->getComponent<Transform>()->getPos() - tr_->getPos();
	dir = dir.normalize() * speed_;
	newDir = dir; //Se aplica el vector dirección
}

Follow::~Follow()
{
}
