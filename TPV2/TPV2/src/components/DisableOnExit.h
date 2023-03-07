#pragma once
#include "Component.h"
#include "Transform.h"
#include "../ecs/Entity.h"
#include "../utils/Vector2D.h"
struct DisableOnExit : public Component
{
public:
	DisableOnExit(int w, int h);
	virtual ~DisableOnExit();
	virtual void initComponent();
	virtual void update();
	constexpr static cmpId_type id = ecs::_DISABLEONEXIT;
private:
	Transform* tr_;
	int winWidth_, winHeight_;
};

