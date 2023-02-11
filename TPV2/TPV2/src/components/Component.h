#pragma once
class Manager;
class Entity;
class Component
{
public:
	Component();
	virtual ~Component();
	inline void setContext(Entity* ent, Manager* mngr);
	virtual void initComponent();
	virtual void update();
	virtual void render();
protected:
	Entity* ent_;
	Manager* mngr_;
};

