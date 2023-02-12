#pragma once
#include "Component.h"
#include "Transform.h"
#include "../ecs/Entity.h"
#include "../utils/Vector2D.h"
#include <iostream>
class DeAcceleration : public Component
{
public:
	DeAcceleration();
	DeAcceleration(float r);
	virtual ~DeAcceleration();
	virtual void initComponent();
	virtual void update();
private:
	Transform* tr_;
	float reduction, stopMargin = 0.001;
};

