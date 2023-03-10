#pragma once
#include "Component.h"
#include "Transform.h"
#include "../ecs/Entity.h"
#include "../utils/Vector2D.h"
struct Follow : public Component
{
public:
	Follow(Entity* f, float s);
	virtual ~Follow();
	virtual void initComponent();
	constexpr static cmpId_type id = ecs::_FOLLOW;

#ifdef COMPS
	virtual void update();
#endif // COMPS

private:
	Transform* tr_;
	Entity* objective_;
	float speed_;
};

