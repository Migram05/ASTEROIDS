#pragma once
#include "../ecs/Component.h"
#include "Transform.h"
#include "../ecs/Entity.h"
#include "../utils/Vector2D.h"
struct DisableOnExit : public Component
{
public:
	DisableOnExit(int w, int h);
	virtual ~DisableOnExit();
	virtual void initComponent();
	constexpr static cmpId_type id = ecs::_DISABLEONEXIT;
#ifdef COMPS
	virtual void update();
#endif // COMPS
private:
	Transform* tr_;
	int winWidth_, winHeight_;
};

