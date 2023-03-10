#pragma once
#include "Component.h"
#include "Transform.h"
#include "../ecs/Entity.h"
#include "../utils/Vector2D.h"

#include <iostream>
struct DeAcceleration : public Component
{
public:
	DeAcceleration();
	DeAcceleration(float r);
	virtual ~DeAcceleration();
	inline float getReduction() { return reduction; }
	inline float getStopMargin() { return stopMargin; }
	virtual void initComponent();
	constexpr static cmpId_type id = ecs::_DEACCELERATION;

#ifdef COMPS
	virtual void update();
#endif // COMPS
private:
	Transform* tr_;
	float reduction, stopMargin = 0.001;
};

