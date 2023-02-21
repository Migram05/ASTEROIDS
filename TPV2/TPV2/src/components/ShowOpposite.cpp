#include "ShowOpposite.h"

ShowOpposite::ShowOpposite(int w, int h) : winWidth_(w), winHeight_(h) //Constructora 
{
}

void ShowOpposite::initComponent()
{
	tr_ = ent_->getComponent<Transform>(); //B�squeda del transform
}

void ShowOpposite::update()
{
	//Si la posici�n supera los l�mites de la pantalla, se env�a a la posici�n opuesta (efecto toroidal)
	Vector2D& position = tr_->getPos();
	if (position.getX() + tr_->getW() < 0) position = Vector2D{ (float)winWidth_ , position.getY()};
	else if(position.getX() > winWidth_) position = Vector2D{ 0 , position.getY()};

	if (position.getY() + tr_->getH() < 0) position = Vector2D{ position.getX() ,(float) winHeight_ };
	else if (position.getY() > winHeight_) position = Vector2D{ position.getX() , 0 };
}

ShowOpposite::~ShowOpposite()
{

}
