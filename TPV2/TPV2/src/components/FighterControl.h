#pragma once
#include <SDL.h>
#include <iostream>
#include "Component.h"
#include "Transform.h"
#include "../ecs/Entity.h"
#include "../utils/Vector2D.h"
#include "../game/ecs.h"
#include "Gun.h"
class FighterControl : public Component
{
public:
	FighterControl();
	FighterControl(float s, float rS);
	virtual ~FighterControl();
	virtual void initComponent();
	virtual void update();
	constexpr static cmpId_type id = ecs::_CTRL;
private:
	Transform* tr_;
	float speed, rotationSpeed;
};

