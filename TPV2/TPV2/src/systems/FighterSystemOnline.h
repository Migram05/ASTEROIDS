#pragma once
#pragma once
#include "../ecs/System.h"
class MultiplayerState;
class FighterSystemOnline : public System
{
public:
	// Reaccionar a los mensajes recibidos (llamando a m�todos correspondientes).
	void receive(const Message& m) override;
	// Crear la entidad del caza, a�adir sus componentes, asociarla con un handler
	// correspondiente, etc.
	void initSystem() override;
	// Si el juego est� parado no hacer nada, en otro caso actualizar la velocidad
	// del caza y moverlo como en la pr�ctica 1 (acelerar, desacelerar, etc). Adem�s, 
	// si el juego no est� parado y el jugador pulsa la tecla de disparo, enviar un
	// mensaje con las caracter�sticas f�sicas de la bala. Recuerda que se puede disparar
	// s�lo una bala cada 0.25sec.
	void update() override;
	void movePlayer(int index);
	void moveAllPlayers();
	constexpr static sysId_type id = ecs::_sys_FIGHTER; //Identificador del tipo de sistema
private:
	// Para reaccionar al mensaje de que ha habido un choque entre el fighter y un
	// un asteroide. Poner el caza en el centro con velocidad (0,0) y rotaci�n 0. No
	// hace falta desactivar la entidad (no dibujarla si el juego est� parado).
	void onCollision_FighterAsteroid();
	void updatePosition();
	void speedReduction();
	void screenPositionCheck();
	//Variables del caza
	float speed = 1.2, rotationSpeed = 5;
	int nPlayers = 2;
	Entity* p;
	MultiplayerState* currentState;
};