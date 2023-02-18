#pragma once
#include "Component.h"
#include "Transform.h"
#include "../ecs/Entity.h"
#include "../utils/Vector2D.h"
class Follow : public Component
{
public:
	Follow(Entity* f, float s);
	virtual ~Follow();
	virtual void initComponent();
	virtual void update();
	constexpr static cmpId_type id = ecs::_FOLLOW;
private:
	Transform* tr_;
	Entity* objective_;
	float speed_;
};

