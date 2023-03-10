#include "DisableOnExit.h"
#include "../ecs/Manager.h"
DisableOnExit::DisableOnExit(int w, int h) : winWidth_(w), winHeight_(h) //Constructora
{
}

DisableOnExit::~DisableOnExit()
{
}

void DisableOnExit::initComponent()
{
	tr_ = mngr_->getComponent<Transform>(ent_); //Se busca el transform
}

#ifdef COMPS
void DisableOnExit::update()
{
	Vector2D& position = tr_->getPos(); //Se obtiene la posición
	//SI sale de la pantalla, se desactiva
	if (position.getX() + tr_->getW() < 0 || position.getX() > winWidth_ || position.getY() + tr_->getH() < 0 || position.getY() > winHeight_) mngr_->setAlive(ent_, false);
}
#endif // COMPS


