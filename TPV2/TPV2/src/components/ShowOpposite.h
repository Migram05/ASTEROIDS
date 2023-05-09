#pragma once
#include "../ecs/Component.h"
#include "Transform.h"
#include "../ecs/Entity.h"
#include "../utils/Vector2D.h"
struct ShowOpposite : public Component
{
public:
	ShowOpposite(int w, int h);
	virtual ~ShowOpposite();
	virtual void initComponent();
	constexpr static cmpId_type id = ecs::_SHOWOPOSITE;

#ifdef COMPS
	virtual void update();
#endif // COMPS

private:
	Transform* tr_;
	int winWidth_, winHeight_;
};

