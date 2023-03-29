#include "MainMenuState.h"
#include "Manager.h"
#include "../systems/RenderSystem.h"
MainMenuState::MainMenuState(Game* g, double w, double h) : GameState(w, h) { // Constructora
	game = g;
}
const std::string MainMenuState::s_mainMID = "MAINMENU";//ID del estado

void MainMenuState::update()
{
#ifdef COMPS
	manager_->update(); //Llamada al manager
#endif // COMPS
#ifndef COMPS

	manager_->flushMessages();
	//El refresh es un método propio de Game State
#endif // !COMPS
}

void MainMenuState::render()
{
#ifdef COMPS
	manager_->render();
#endif // COMPS
#ifndef COMPS
	renderSys_->update();
#endif // !COMPS
}

bool MainMenuState::onEnter()
{
	manager_ = new Manager(game);
#ifdef COMPS
	cout << "Using components" << endl;
#endif // COMPS

#ifndef COMPS
	cout << "Using systems" << endl;

	renderSys_ = manager_->addSystem<RenderSystem>();
#endif // !COMPS
	return true;
}

void MainMenuState::refresh()
{
}


MainMenuState::~MainMenuState()
{

}