#include "System.h"
#include "Manager.h"
System::System()
{
}

System::~System()
{
}

void System::setContext(Manager* mngr)
{
	mngr_ = mngr;
}

void System::initSystem()
{
}

void System::update()
{
}

void System::receive(const Message& m)
{
}
