#include "Button.h"
#include "../game/Game.h"
#include "../ecs/MainMenuState.h"
#include "../ecs/Manager.h"
Button::Button(void(*callback)(Game* g), Game* g) : buttonCallback(callback), game(g)
{
}
Button::~Button()
{
}
void Button::initComponent()
{
}
#ifdef COMPS
void Button::update()
{
	bool stop = false;
	SDL_Event event;
	SDL_Rect destRect = mngr_->getComponent<Transform>(ent_)->getRect();
	while (!stop && SDL_PollEvent(&event)) { //Controla el input
		SDL_Point mPoint; //Se guarda la posición del cursor
		if (event.button.button == SDL_BUTTON_LEFT && event.type == SDL_MOUSEBUTTONUP) { //Input del ratón en el menú
			mPoint.x = event.button.x; mPoint.y = event.button.y;
			stop = true;
			if (SDL_PointInRect(&mPoint, &destRect)) buttonCallback(game);
			else SDL_PushEvent(&event);
		}
	}
}
#endif // COMPS

