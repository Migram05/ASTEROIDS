#include "PauseState.h"
#include "../game/Game.h"
PauseState::PauseState(Game* g, double w, double h, bool l) : game(g), GameState(w, h), lost(l) { // Constructora 

}
const std::string PauseState::s_pauseID = "PAUSE";//ID del estado

void PauseState::update() //Se detecta si hay entrada para cambiar de estado o salir
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_KEYDOWN) {
			switch (event.key.keysym.sym)
			{
			case SDLK_SPACE: game->resumeGame(lost); break; //Al pulsar espacio se sale de la pausa
			case SDLK_ESCAPE: game->exitToMenu(""); break; //Al pulsar Escape, se sale del juego
			default: break;
			}
		}
		
	}
}

void PauseState::render() //Renderizado
{
	auto& sdl = *SDLUtils::instance();
	if (lost) { //En caso de perder se escribe el texto de derrota
		Texture lostText(sdl.renderer(), "GAME OVER", sdl.fonts().at("CAPTURE50"), build_sdlcolor(0x05FAFFff));
		lostText.render(sdl.width() / 2 - 125, sdl.height() / 2 - 150);
	}
	//Siempre se muestra el texto de pulsar "Espacio" para continuar
	Texture pressAnyKey(sdl.renderer(), "PRESS SPACE TO CONTINUE", sdl.fonts().at("ARIAL18"), build_sdlcolor(0x112233ff), build_sdlcolor(0xffffffff));
	pressAnyKey.render(sdl.width() / 2 -120, sdl.height() / 2 +50);
}

bool PauseState::onEnter() //Llamado al entrar en el estado
{
	return true;
}

PauseState::~PauseState() {

}