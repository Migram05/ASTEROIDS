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
	Entity* e = manager_->addEntity();
	e->setContext(manager_);
	e->addComponent<Transform>(ecs::_TRANSFORM, 100,100, 30,30);
	e->addComponent<Image>(ecs::_IMAGE, game->getTexture(2));
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
