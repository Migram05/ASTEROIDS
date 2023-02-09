#include "PlayState.h"
#include "../game/Game.h"
PlayState::PlayState(Game* g, double w, double h) : GameState(w,h){ // Constructora
	game = g;
}
const std::string PlayState::s_playID = "PLAY";//ID del estado

void PlayState::update() //Se actualizan los objetos de la lista
{

}
void PlayState::render() //Renderizado del juego
{

}
void PlayState::handleEvents() { //Se hace el handle events del paddle y se leen algunas teclas
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		//if (event.type == SDL_QUIT) game->exitGame();
		//else if (event.key.keysym.sym == SDLK_ESCAPE && event.type == SDL_KEYDOWN) game->pauseGame();
	}
}
bool PlayState::onEnter() //Se inicializan los objetos
{
	return true;
}

PlayState::~PlayState() {
	for (ObjIt = objectsList.begin(); ObjIt != objectsList.end(); ++ObjIt) { //Se borrran los objetos
		delete* ObjIt;
	}
	objectsList.clear();
}
