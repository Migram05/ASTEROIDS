#include "PlayState.h"
#include "../game/Game.h"
PlayState::PlayState(Game* g, double w, double h) : GameState(w,h){ // Constructora
	game = g;
}
const std::string PlayState::s_playID = "PLAY";//ID del estado

void PlayState::update() //Se actualizan los objetos de la lista
{
	manager_->update();
}
void PlayState::render() //Renderizado del juego
{
	manager_->render();
}
bool PlayState::onEnter() //Se inicializan los objetos
{
	manager_ = new Manager();
	return true;
}

void PlayState::refresh()
{
	manager_->refresh();
}

PlayState::~PlayState() {
	for (ObjIt = objectsList.begin(); ObjIt != objectsList.end(); ++ObjIt) { //Se borrran los objetos
		delete* ObjIt;
	}
	objectsList.clear();
	delete manager_;
}
