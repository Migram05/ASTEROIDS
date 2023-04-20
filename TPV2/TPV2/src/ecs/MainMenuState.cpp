#include "MainMenuState.h"
#include "Manager.h"
#include "../systems/RenderSystem.h"
#include "../systems/MenuControlSystem.h"
MainMenuState::MainMenuState(Game* g, double w, double h, string info) : GameState(w, h) { // Constructora
	game = g;
	buttonW = w / 10;
	buttonH = h / 15;
	txtBoxH = 1.2 * buttonH;
	txtBoxW = 5 * buttonW;
	infoText = info;
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
}

void MainMenuState::render()
{
#ifdef COMPS
	manager_->render();
#endif // COMPS
#ifndef COMPS
	renderSys_->update();
#endif // !COMPS
	auto& sdl = *SDLUtils::instance();
	if (infoText != "") {
		Texture ipDirText(sdl.renderer(), infoText, sdl.fonts().at("CAPTURE10"), build_sdlcolor(0xffffffff));
		ipDirText.render(manager_->getWidth() * 0.3, manager_->getHeight() * 0.9);
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
	manager_->refresh();
}

void MainMenuState::createButtons()
{
	auto& sdl = *SDLUtils::instance();
	//Botón de 1 jugador
	auto p1 = manager_->addEntity(ecs::_grp_UI);
	manager_->addComponent<Transform>(p1, Vector2D(WIN_WIDTH / 2 - buttonW / 2, WIN_HEIGHT / 2 - buttonH * 3), buttonW, buttonH);
	manager_->addComponent<Image>(p1, &sdl.images().at("singleB"));
	manager_->addComponent<Button>(p1, startSingleplayer, game);

	//Botón de multijugador
	auto p2 = manager_->addEntity(ecs::_grp_UI);
	manager_->addComponent<Transform>(p2, Vector2D(WIN_WIDTH / 2 - buttonW / 2, WIN_HEIGHT / 2 - buttonH / 2), buttonW, buttonH);
	manager_->addComponent<Image>(p2, &sdl.images().at("multiB"));
	manager_->addComponent<Button>(p2, enterName, game);

	//Botón de salida
	auto exitB = manager_->addEntity(ecs::_grp_UI);
	manager_->addComponent<Transform>(exitB, Vector2D(WIN_WIDTH / 2 - buttonW / 2, WIN_HEIGHT / 2 + buttonH * 2), buttonW, buttonH);
	manager_->addComponent<Image>(exitB, &sdl.images().at("exitB"));
	manager_->addComponent<Button>(exitB, exitGame, game);

	//Botón de crear partida
	auto hB = manager_->addEntity(ecs::_grp_UI, false);
	manager_->addComponent<Transform>(hB, Vector2D(WIN_WIDTH / 2 + buttonW , WIN_HEIGHT / 2 - buttonH), buttonW, buttonH);
	manager_->addComponent<Image>(hB, &sdl.images().at("hostB"));
	manager_->addComponent<Button>(hB, hostMultiplayer, game);

	//Botón de unirse a partida
	auto jB = manager_->addEntity(ecs::_grp_UI, false);
	manager_->addComponent<Transform>(jB, Vector2D(WIN_WIDTH / 2 + buttonW , WIN_HEIGHT / 2 + buttonH/2), buttonW, buttonH);
	manager_->addComponent<Image>(jB, &sdl.images().at("joinB"));
	manager_->addComponent<Button>(jB, searchMultiplayer, game);
}

void MainMenuState::startSingleplayer(Game* g)
{
	g->playSingleplayer();
}

void MainMenuState::hostMultiplayer(Game* g)
{
	static_cast<MainMenuState*>(g->getState())->startMultiplayer(false, "localhost");
}

void MainMenuState::searchMultiplayer(Game* g)
{
	static_cast<MainMenuState*>(g->getState())->startRead("ESCRIBA LA DIRECCION IP:");
}

void MainMenuState::startMultiplayer(bool c, string dir)
{
	if (dir == "") dir = "localhost";
	game->playMultiplayer(c, dir, manager_->getPlayerName());
}

void MainMenuState::exitGame(Game* g)
{
	g->exitGame();
}

void MainMenuState::enterName(Game* g)
{
	static_cast<MainMenuState*>(g->getState())->startRead("ESCRIBA NOMBRE DE USUARIO:", false);
}

void MainMenuState::showButtons()
{
	
	for (Entity* e : manager_->getEntitiesByGroup(ecs::_grp_UI)) {
		if (!e->isVisible()) e->setVisibility(true);
	}
}

void MainMenuState::startRead(string dTxt, bool readN)
{
	//Cuadro de texto
	textBoxEnt = manager_->addEntity(ecs::_grp_UI);
	manager_->addComponent<Transform>(textBoxEnt, Vector2D((WIN_WIDTH / 2) - (txtBoxW/2), WIN_HEIGHT * 0.8), txtBoxW, txtBoxH);
	manager_->addComponent<Image>(textBoxEnt, manager_->getTexture(TextBox));
	manager_->addComponent<TextBoxComponent>(textBoxEnt, this, Vector2D(((WIN_WIDTH / 2) - (txtBoxW / 2))*1.1, (WIN_HEIGHT * 0.8) + txtBoxH/2), dTxt, readN);
}

MainMenuState::~MainMenuState()
{
	delete manager_;
}