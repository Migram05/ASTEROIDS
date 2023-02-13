#include "Transform.h"

Transform::Transform()
{
}

Transform::Transform(Vector2D pos, float w, float h) : position_(pos), width_(w), height_(h)
{
	
}

Transform::Transform(float x, float y, float w, float h) : position_(Vector2D(x,y)), width_(w), height_(h)
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

void Transform::update()
{
	position_ = position_ + velocity_;
}

void Transform::render()
{
}

Transform::~Transform()
{

}
