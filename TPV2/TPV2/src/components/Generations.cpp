#include "Generations.h"
#include "../ecs/Manager.h"
Generations::Generations() : numGen(3) //Constructora por defecto
{
}

Generations::Generations(int n) : numGen(n) //Constructora por valor
{
}

void Generations::initComponent() //Se inicializa el componente
{
	tr_ = mngr_->getComponent<Transform>(ent_);
	float& w = tr_->getW();
	float& h = tr_->getH();
	w = w * numGen; h = h * numGen; //Se ajusta el tamaño de la entidad según la generación
}

