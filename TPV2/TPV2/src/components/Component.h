#pragma once
#include "../game/ecs.h"

class Manager;
class Entity;
using cmpId_type = uint8_t;

class Component
{
public:
	Component();
	virtual ~Component();
	inline void setContext(Entity* ent, Manager* mngr) {
		ent_ = ent;
		mngr_ = mngr;
	}
	virtual void initComponent();
	virtual void update();
	virtual void render();
	constexpr static cmpId_type id = ecs::maxComponentId;
protected:
	Entity* ent_;
	Manager* mngr_;
};

