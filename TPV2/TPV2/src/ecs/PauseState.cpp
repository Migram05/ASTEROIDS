#include "PauseState.h"
#include "../game/Game.h"
PauseState::PauseState(Game* g, double w, double h, bool l) : GameState(w, h), lost(l) { // Constructora
	game = g;
}
const std::string PauseState::s_pauseID = "PAUSE";//ID del estado

void PauseState::update() //Se actualizan los objetos de la lista
{
	SDL_Event event;
	if (SDL_PollEvent(&event) && event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
		game->resumeGame();
	}
}

void PauseState::render()
{
	auto& sdl = *SDLUtils::instance();
	Font f("../TPV2/resources/fonts/ARIAL.ttf", 18);
	Font f2("../TPV2/resources/fonts/Capture_it.ttf", 50);
	if (lost) {
		Texture lostText(sdl.renderer(), "GAME OVER", f2, build_sdlcolor(0x112233ff));
		lostText.render(sdl.width() / 2 - 125, sdl.height() / 2 - 150);
	}
	Texture pressAnyKey(sdl.renderer(), "PRESS SPACE TO CONTINUE", f, build_sdlcolor(0x112233ff), build_sdlcolor(0xffffffff));
	pressAnyKey.render(sdl.width() / 2 -120, sdl.height() / 2 +50);
}

bool PauseState::onEnter() //Se inicializan los objetos
{
	
	return true;
}

PauseState::~PauseState() {

}