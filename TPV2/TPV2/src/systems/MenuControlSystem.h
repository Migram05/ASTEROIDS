#pragma once
#include "../ecs/System.h"
using Uint8 = uint8_t;
class Game;
class MenuControlSystem : public System
{
public:
	// Reaccionar a los mensajes recibidos (llamando a métodos correspondientes).
	void receive(const Message& m) override;
	// Inicializar el sistema, etc.
	void initSystem() override;
	// Si el juego no está parado y el jugador pulsa SDLK_SPACE cambia el estado
	// como en la práctica 1, etc. Tiene que enviar mensajes correspondientes cuando
	// empieza una ronda o cuando empieza una nueva partida.
	void update() override;
	constexpr static sysId_type id = ecs::_sys_GAMECTRL; //Identificador del tipo de sistema
private:
	void (*buttonCallback)(Game* g);
};
