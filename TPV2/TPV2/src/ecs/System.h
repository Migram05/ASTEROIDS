#pragma once
#include "../message/Message.h"
class Manager;
using sysId_type = uint8_t;
class System //Clase de sistemas
{
public:
	System();
	virtual ~System();
	void setContext(Manager* mngr);
	virtual void initSystem();
	virtual void update();
	virtual void receive(const Message& m); //Método para recibir un mensaje
	constexpr static sysId_type id = ecs::maxSystemId; //Identificador del tipo de sistema
protected:
	Manager* mngr_;
};

