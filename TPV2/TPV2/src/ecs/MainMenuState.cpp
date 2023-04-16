#include "MainMenuState.h"
#include "Manager.h"
#include "../systems/RenderSystem.h"
#include "../systems/MenuControlSystem.h"
MainMenuState::MainMenuState(Game* g, double w, double h) : GameState(w, h) { // Constructora
	game = g;
	buttonW = w / 10;
	buttonH = h / 15;
	txtBoxH = 1.2 * buttonH;
	txtBoxW = 5 * buttonW;
}
const std::string MainMenuState::s_mainMID = "MAINMENU";//ID del estado

void MainMenuState::update()
{
#ifdef COMPS
	manager_->update(); //Llamada al manager
#endif // COMPS
#ifndef COMPS

	manager_->flushMessages();
	menuCtrlSys_->update();
	//El refresh es un m�todo propio de Game State
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
	//Crea los botones de la interfaz
	createButtons();
#ifdef COMPS
	cout << "Using components" << endl;
#endif // COMPS

#ifndef COMPS
	cout << "Using systems" << endl;

	renderSys_ = manager_->addSystem<RenderSystem>();
	menuCtrlSys_ = manager_->addSystem<MenuControlSystem>();
#endif // !COMPS
	return true;
}

void MainMenuState::refresh()
{
}

void MainMenuState::createButtons()
{
	//Bot�n de 1 jugador
	auto p1 = manager_->addEntity(ecs::_grp_UI);
	manager_->addComponent<Transform>(p1, Vector2D(WIN_WIDTH / 2 - buttonW / 2, WIN_HEIGHT / 2 - buttonH * 3), buttonW, buttonH);
	manager_->addComponent<Image>(p1, manager_->getTexture(OnePlayer));
	manager_->addComponent<Button>(p1, startSingleplayer, game);

	//Bot�n de multijugador
	auto p2 = manager_->addEntity(ecs::_grp_UI);
	manager_->addComponent<Transform>(p2, Vector2D(WIN_WIDTH / 2 - buttonW / 2, WIN_HEIGHT / 2 - buttonH / 2), buttonW, buttonH);
	manager_->addComponent<Image>(p2, manager_->getTexture(Multiplayer));
	manager_->addComponent<Button>(p2, showButtonsClbck, game);

	//Bot�n de salida
	auto exitB = manager_->addEntity(ecs::_grp_UI);
	manager_->addComponent<Transform>(exitB, Vector2D(WIN_WIDTH / 2 - buttonW / 2, WIN_HEIGHT / 2 + buttonH * 2), buttonW, buttonH);
	manager_->addComponent<Image>(exitB, manager_->getTexture(Exit));
	manager_->addComponent<Button>(exitB, exitGame, game);

	//Bot�n de crear partida
	auto hB = manager_->addEntity(ecs::_grp_UI, false);
	manager_->addComponent<Transform>(hB, Vector2D(WIN_WIDTH / 2 + buttonW , WIN_HEIGHT / 2 - buttonH), buttonW, buttonH);
	manager_->addComponent<Image>(hB, manager_->getTexture(Host));
	manager_->addComponent<Button>(hB, hostMultiplayer, game);

	//Bot�n de unirse a partida
	auto jB = manager_->addEntity(ecs::_grp_UI, false);
	manager_->addComponent<Transform>(jB, Vector2D(WIN_WIDTH / 2 + buttonW , WIN_HEIGHT / 2 + buttonH/2), buttonW, buttonH);
	manager_->addComponent<Image>(jB, manager_->getTexture(Join));
	manager_->addComponent<Button>(jB, searchMultiplayer, game);
}

void MainMenuState::startSingleplayer(Game* g)
{
	g->playSingleplayer();
}

void MainMenuState::hostMultiplayer(Game* g)
{
	g->playMultiplayer(false);
}

void MainMenuState::searchMultiplayer(Game* g)
{
	static_cast<MainMenuState*>(g->getState())->startRead();
}

void MainMenuState::startMultiplayer(string dir)
{
	if (dir == "") dir = "localhost";
	game->playMultiplayer(true, dir);
}

void MainMenuState::exitGame(Game* g)
{
	g->exitGame();
}

void MainMenuState::showButtonsClbck(Game* g)
{
	static_cast<MainMenuState*>(g->getState())->showButtons();
}

void MainMenuState::showButtons()
{
	for (Entity* e : manager_->getEntitiesByGroup(ecs::_grp_UI)) {
		if (!e->isVisible()) e->setVisibility(true);
	}
}

void MainMenuState::startRead()
{
	//Cuadro de texto
	textBoxEnt = manager_->addEntity(ecs::_grp_UI);
	manager_->addComponent<Transform>(textBoxEnt, Vector2D((WIN_WIDTH / 2) - (txtBoxW/2), WIN_HEIGHT * 0.8), txtBoxW, txtBoxH);
	manager_->addComponent<Image>(textBoxEnt, manager_->getTexture(TextBox));
	manager_->addComponent<TextBoxComponent>(textBoxEnt, this, Vector2D(((WIN_WIDTH / 2) - (txtBoxW / 2))*1.1, (WIN_HEIGHT * 0.8) + txtBoxH/2));
}

MainMenuState::~MainMenuState()
{
	delete manager_;
}