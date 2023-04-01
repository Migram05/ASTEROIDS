#include "MenuControlSystem.h"
#include "../ecs/Manager.h"
void MenuControlSystem::receive(const Message& m)
{

}

void MenuControlSystem::initSystem() //Al crear env�a un mensaje de comienzo de partida
{
	Message msg;
	msg.id = _m_NEWGAME;
	mngr_->send(msg, true);
}

void MenuControlSystem::update()
{
	bool stop = false; //Detiene el bucle si se ejecuta una acci�n
	SDL_Event event;
	while (!stop && SDL_PollEvent(&event)) { //Controla el input
		SDL_Point mPoint; //Se guarda la posici�n del cursor
		if (event.button.button == SDL_BUTTON_LEFT && event.type == SDL_MOUSEBUTTONUP) { //Input del rat�n en el men�
			mPoint.x = event.button.x; mPoint.y = event.button.y;
			for (auto e : mngr_->getEntitiesByGroup(ecs::_grp_UI)) { //Busca entre todos los elementos de la UI
				SDL_Rect destRect = mngr_->getComponent<Transform>(e)->getRect();
				auto but_ = mngr_->getComponent<Button>(e);
				if (SDL_PointInRect(&mPoint, &destRect)) { //Si ha hecho clic en un bot�n:
					buttonCallback = but_->getEvent();
					buttonCallback(mngr_->getGame()); //Ejecuta la acci�n
					stop = true; //Para la b�squeda
				}
				else SDL_PushEvent(&event);
			}
		}
	}
}
