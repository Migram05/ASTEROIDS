#include "MenuControlSystem.h"
#include "../ecs/Manager.h"
#include "../ecs/MainMenuState.h"
void MenuControlSystem::receive(const Message& m)
{

}

void MenuControlSystem::initSystem() //Al crear envía un mensaje de comienzo de partida
{
	Message msg;
	msg.id = _m_NEWGAME;
	mngr_->send(msg, true);
	currentState = static_cast<MainMenuState*>(mngr_->getGame()->getState());
}

void MenuControlSystem::update()
{
	bool stop = false; //Detiene el bucle si se ejecuta una acción
	SDL_Event event;
	while (!stop && SDL_PollEvent(&event)) { //Controla el input
		SDL_Point mPoint; //Se guarda la posición del cursor
		if (event.button.button == SDL_BUTTON_LEFT && event.type == SDL_MOUSEBUTTONUP) { //Input del ratón en el menú
			mPoint.x = event.button.x; mPoint.y = event.button.y;
			for (auto e : mngr_->getEntitiesByGroup(ecs::_grp_UI)) { //Busca entre todos los elementos de la UI
				SDL_Rect destRect = mngr_->getComponent<Transform>(e)->getRect();
				auto but_ = mngr_->getComponent<Button>(e);
				if (e->isVisible() && SDL_PointInRect(&mPoint, &destRect)) { //Si ha hecho clic en un botón:
					buttonCallback = but_->getEvent();
					buttonCallback(mngr_->getGame()); //Ejecuta la acción
					stop = true; //Para la búsqueda
				}
			}
		}
		else if (event.type == SDL_KEYDOWN) { //Para el elemento de textBox
			auto entity = currentState->getTextBox(); //Obtiene la textBox del menú
			if (!entity) return;
			string& text = mngr_->getComponent<TextBoxComponent>(entity)->getText();
			char c = event.key.keysym.sym;
			if (event.key.keysym.sym - 13 == 0) { //Cálculo para saber si es la tecla ENTER
				currentState->startMultiplayer(text);
			}
			if (event.key.keysym.sym == SDLK_BACKSPACE && !text.empty()) {
				auto it = text.end(); --it;
				text.erase(it);
			}
			if (isdigit(c) || c == '.') text += c; //Solo si es un número se añade
		}
	}
}
