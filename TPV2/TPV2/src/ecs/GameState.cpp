#include "GameState.h"
#include "../game/Game.h"
GameState::GameState(double w, double h) { //Constructora de estado de juego
	WIN_WIDTH = w;
	WIN_HEIGHT = h;
	
}
void GameState::update() {
	for (GameObject* g : objectsList) g->update();
}
void GameState::render() {
	for (GameObject* g : objectsList) g->render();
}
void GameState::handleEvents() {

}
bool GameState::onEnter() {
	return 0;
}	
string GameState::getStateID() const { //Devuelve el ID
	return " ";
}
GameState::~GameState(){ //Borra todos los elementos de la lista de objetos
	for (GameObject* g : objectsList) delete g;
}