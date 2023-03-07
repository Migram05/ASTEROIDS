#pragma once
#include "../message/Message.h"
class Manager;
class System //Clase de sistemas
{
public:
	System();
	virtual ~System();
	void setContext(Manager* mngr);
	virtual void initSystem();
	virtual void update();
	virtual void receive(const Message& m); //Método para recibir un mensaje
	constexpr static ecs::sysId_type id = ecs::_LAST_SYS_ID; //Identificador del tipo de sistema
protected:
	Manager* mngr_;
};

