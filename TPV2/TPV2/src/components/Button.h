#pragma once
#include "Component.h"
#include "../sdlutils/Texture.h"
#include "Transform.h"
#include "../ecs/Entity.h"
class Game;
class MainMenuState;
class Button : public Component
{
public:
	Button(void (*callback)(Game* g), Game* g);

	virtual ~Button();
	void initComponent() override;
	auto getEvent() { return buttonCallback; } //Devuelve el evento del botón
	constexpr static cmpId_type id = ecs::_BUTTON;

#ifdef COMPS
	void update() override;
#endif // COMPS

private:
	Game* game;
	void (*buttonCallback)(Game* g);
};
