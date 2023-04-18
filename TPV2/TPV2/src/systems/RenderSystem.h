#pragma once
#include "../ecs/System.h"
#include "../sdlutils/Texture.h"
using Uint8 = uint8_t;
class RenderSystem : public System
{
public:
	// Reaccionar a los mensajes recibidos (llamando a métodos correspondientes).
	void receive(const Message& m) override;
		// Inicializar el sistema, etc.
	void initSystem() override;
		// - Dibujar asteroides, balas y caza (sólo si el juego no está parado).
		// - Dibujar las vidas (siempre).
		// - Dibujar los mensajes correspondientes: si el juego está parado, etc (como en
		// la práctica 1)
	void update() override;
	constexpr static sysId_type id = ecs::_sys_RENDER; //Identificador del tipo de sistema
private:
	void showAllEntities();
	Texture* tex_;
	Texture* hTex_;
};

