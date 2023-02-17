#include "PauseState.h"
#include "../game/Game.h"
PauseState::PauseState(Game* g, double w, double h) : GameState(w, h) { // Constructora
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

bool PauseState::onEnter() //Se inicializan los objetos
{
	
	return true;
}

PauseState::~PauseState() {

}