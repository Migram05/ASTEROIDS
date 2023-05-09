#pragma once
#include "../game/ecs.h"
//Si no se comenta, se ejecuta el juego por componentes en vez de sistemas
//#define COMPS

class Manager;
class Entity;
using cmpId_type = uint8_t;

struct Component //Clase Padre
{
public:
	Component();
	virtual ~Component();
	inline void setContext(Entity* ent, Manager* mngr) {
		ent_ = ent;
		mngr_ = mngr;
	}
	virtual void initComponent();
#ifdef COMPS
	virtual void update();
	virtual void render();
#endif // COMPS
	constexpr static cmpId_type id = ecs::maxComponentId;
protected:
	Entity* ent_;
	Manager* mngr_;
};

