#pragma once
#include "Component.h"
#include "Transform.h"
#include "../ecs/Entity.h"
#include "../utils/Vector2D.h"
class ShowOpposite : public Component
{
public:
	ShowOpposite(int w, int h);
	virtual ~ShowOpposite();
	virtual void initComponent();
	virtual void update();
	constexpr static cmpId_type id = ecs::_SHOWOPOSITE;
private:
	Transform* tr_;
	int winWidth_, winHeight_;
};

