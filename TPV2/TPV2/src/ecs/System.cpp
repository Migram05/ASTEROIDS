#include "System.h"
#include "Manager.h"
System::System()
{
}


void System::setContext(Manager* mngr) //Obtiene la referencia del manager actual
{
	mngr_ = mngr;
}

void System::initSystem() //Inicializa el sistema
{
}

void System::update() //Actualiza entidades, tambi�n usado para el render
{
}

void System::receive(const Message& m) //M�todo para recibir mensajes
{
}

System::~System()
{
}