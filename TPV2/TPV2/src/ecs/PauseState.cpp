#include "PauseState.h"
#include "../game/Game.h"
PauseState::PauseState(Game* g, double w, double h, bool l) : GameState(w, h), lost(l) { // Constructora
	game = g;
}
const std::string PauseState::s_pauseID = "PAUSE";//ID del estado

void PauseState::update() //Se actualizan los objetos de la lista
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_KEYDOWN) {
			switch (event.key.keysym.sym)
			{
			case SDLK_SPACE: game->resumeGame(); break;
			case SDLK_ESCAPE: game->exitGame(); break;
			default: break;
			}
		}
		
	}
}

void PauseState::render()
{
	auto& sdl = *SDLUtils::instance();
	if (lost) {
		Texture lostText(sdl.renderer(), "GAME OVER", sdl.fonts().at("CAPTURE50"), build_sdlcolor(0x112233ff));
		lostText.render(sdl.width() / 2 - 125, sdl.height() / 2 - 150);
	}
	Texture pressAnyKey(sdl.renderer(), "PRESS SPACE TO CONTINUE", sdl.fonts().at("ARIAL18"), build_sdlcolor(0x112233ff), build_sdlcolor(0xffffffff));
	pressAnyKey.render(sdl.width() / 2 -120, sdl.height() / 2 +50);
}

bool PauseState::onEnter() //Se inicializan los objetos
{
	return true;
}

PauseState::~PauseState() {

}