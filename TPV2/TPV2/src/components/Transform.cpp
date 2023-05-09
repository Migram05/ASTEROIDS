#include "Transform.h"

//Constructoras por valor y por defecto
Transform::Transform()
{
}

Transform::Transform(Vector2D pos, float w, float h, float rot) : position_(pos), width_(w), height_(h), rotation_(rot) //Constructora
{
	
}

Transform::Transform(float x, float y, float w, float h, float rot) : position_(Vector2D(x,y)), width_(w), height_(h), rotation_(rot)
{
}

Transform::Transform(Vector2D pos, float w, float h, Vector2D vel, float rot) : position_(pos), width_(w), height_(h), velocity_(vel), rotation_(rot)
{
}

Transform::Transform(float x, float y, float w, float h, Vector2D vel, float rot) : position_(Vector2D(x, y)), width_(w), height_(h), velocity_(vel), rotation_(rot)
{
}

void Transform::initComponent()
{
}

Transform::~Transform()
{

}

#ifdef COMPS

void Transform::update()
{
	position_ = position_ + velocity_; //Se ajusta la posición a la velocidad que lleva
}

void Transform::render()
{
}
#endif // COMPS
