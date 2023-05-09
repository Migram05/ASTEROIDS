#pragma once
#include <SDL.h>
#include <iostream>
#include "../ecs/Component.h"
#include "Transform.h"
#include "../ecs/Entity.h"
#include "../utils/Vector2D.h"
#include "../game/ecs.h"
#include "Gun.h"
struct FighterControl : public Component
{
public:
	FighterControl();
	FighterControl(float s, float rS);
	virtual ~FighterControl();
	virtual void initComponent();
	constexpr static cmpId_type id = ecs::_CTRL;
#ifdef COMPS
	virtual void update();
#endif // COMPS
private:
	Transform* tr_;
	float speed = 0.2, rotationSpeed = 5;
};

