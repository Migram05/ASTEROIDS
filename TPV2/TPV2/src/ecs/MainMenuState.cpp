#include "MainMenuState.h"
#include "Manager.h"
#include "../systems/RenderSystem.h"
#include "../systems/MenuControlSystem.h"
MainMenuState::MainMenuState(Game* g, double w, double h) : GameState(w, h) { // Constructora
	game = g;
	buttonW = w / 10;
	buttonH = h / 15;
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
	//El refresh es un método propio de Game State
#endif // !COMPS
	if (readingKeys) {
		readKeys();
	}
}

void MainMenuState::render()
{
#ifdef COMPS
	manager_->render();
#endif // COMPS
#ifndef COMPS
	renderSys_->update();
#endif // !COMPS
	if (readingKeys) {
		manager_->getTexture(TextBox)->render(WIN_WIDTH/2 -225 , WIN_HEIGHT * 0.8);
	}
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
	//Botón de 1 jugador
	auto p1 = manager_->addEntity(ecs::_grp_UI);
	manager_->addComponent<Transform>(p1, Vector2D(WIN_WIDTH / 2 - buttonW / 2, WIN_HEIGHT / 2 - buttonH * 3), buttonW, buttonH);
	manager_->addComponent<Image>(p1, manager_->getTexture(OnePlayer));
	manager_->addComponent<Button>(p1, startSingleplayer, game);

	//Botón de multijugador
	auto p2 = manager_->addEntity(ecs::_grp_UI);
	manager_->addComponent<Transform>(p2, Vector2D(WIN_WIDTH / 2 - buttonW / 2, WIN_HEIGHT / 2 - buttonH / 2), buttonW, buttonH);
	manager_->addComponent<Image>(p2, manager_->getTexture(Multiplayer));
	manager_->addComponent<Button>(p2, showButtonsClbck, game);

	//Botón de salida
	auto exitB = manager_->addEntity(ecs::_grp_UI);
	manager_->addComponent<Transform>(exitB, Vector2D(WIN_WIDTH / 2 - buttonW / 2, WIN_HEIGHT / 2 + buttonH * 2), buttonW, buttonH);
	manager_->addComponent<Image>(exitB, manager_->getTexture(Exit));
	manager_->addComponent<Button>(exitB, exitGame, game);

	//Botón de crear partida
	auto hB = manager_->addEntity(ecs::_grp_UI, false);
	manager_->addComponent<Transform>(hB, Vector2D(WIN_WIDTH / 2 + buttonW , WIN_HEIGHT / 2 - buttonH), buttonW, buttonH);
	manager_->addComponent<Image>(hB, manager_->getTexture(Host));
	manager_->addComponent<Button>(hB, hostMultiplayer, game);

	//Botón de unirse a partida
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

void MainMenuState::readKeys()
{
	SDL_Event event;
	auto& sdl = *SDLUtils::instance();
	bool exit = false;
	while (!exit) {
		while (SDL_PollEvent(&event)) { //Controla la entrada
			if (event.type == SDL_KEYDOWN) {
				char c = event.key.keysym.sym;
				if (event.key.keysym.sym == SDLK_SPACE) {
					readingKeys = false;
					game->playMultiplayer(true, keysRead);
				}
				if (event.key.keysym.sym == SDLK_BACKSPACE && !keysRead.empty()) {
					auto it = keysRead.end(); --it;
					keysRead.erase(it);
				}
				keysRead += c;
				exit = true;
				cout << keysRead << endl;
			}
		}
	}
	
}

MainMenuState::~MainMenuState()
{
	delete manager_;
}