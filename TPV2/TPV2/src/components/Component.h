#pragma once
#include "../game/ecs.h"

class Manager;
class Entity;
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
protected:
	Entity* ent_;
	Manager* mngr_;
};

