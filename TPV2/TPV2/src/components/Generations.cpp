#include "Generations.h"

Generations::Generations() : numGen(3) //Constructora por defecto
{
}

Generations::Generations(int n) : numGen(n) //Constructora por valor
{
}

void Generations::initComponent() //Se inicializa el componente
{
	tr_ = ent_->getComponent<Transform>();
	float& w = tr_->getW();
	float& h = tr_->getH();
	w = w * numGen; h = h * numGen; //Se ajusta el tamaño de la entidad según la generación
}

