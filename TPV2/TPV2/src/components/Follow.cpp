#include "Follow.h"
#include "../ecs/Manager.h"
Follow::Follow(Entity* f, float s) : objective_(f), speed_(s) //Constructora, se guarda al objetivo a seguir
{
}

void Follow::initComponent()
{
	tr_ = mngr_->getComponent<Transform>(ent_); //Inicializa el componente buscando el transform
}

void Follow::update()
{
	Vector2D& newDir = tr_->getVel();
	//Calcula el nuevo vector dirección
	Vector2D dir = mngr_->getComponent<Transform>(objective_)->getPos() - tr_->getPos();
	dir = dir.normalize() * speed_;
	newDir = dir; //Se aplica el vector dirección
}

Follow::~Follow()
{
}
