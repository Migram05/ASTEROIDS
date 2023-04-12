#pragma once
#include "../ecs/System.h"
class CollisionsSystem : public System
{
public:
	// Reaccionar a los mensajes recibidos (llamando a m�todos correspondientes).
	void receive(const Message& m) override;
		// Inicializar el sistema, etc.
	void initSystem() override;
		// Si el juego est� parado no hacer nada, en otro caso comprobar colisiones como
		// en la pr�ctica 1 y enviar mensajes correspondientes.
	void update() override;
	constexpr static sysId_type id = ecs::_sys_COLLISION; //Identificador del tipo de sistema
private:
	bool isMultiplayer;
};

