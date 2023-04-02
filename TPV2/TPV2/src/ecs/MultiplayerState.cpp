#include "MultiplayerState.h"
#include "Manager.h"
#include "../systems/BulletsSystem.h"
#include "../systems/CollisionsSystem.h"
#include "../systems/FighterSystem.h"
#include "../systems/GameCtrlSystem.h"
#include "../systems/RenderSystem.h"

MultiplayerState::MultiplayerState(Game* g, double w, double h) : GameState(w, h) // Constructora
{
	game = g;
}
const std::string MultiplayerState::s_playID = "MULTIPLAY";//ID del estado

void MultiplayerState::update()
{
#ifdef COMPS

	manager_->update(); //Llamada al manager
	asteroidsManager_->addAsteroidFrequency(); //Se chequea el tiempo para generar o no un asteroide
	checkCollisions(); //Colisiones
#endif // COMPS
#ifndef COMPS

	gameCtrlSys_->update();
	fighterSys_->update();
	bulletSys_->update();
	collisionSys_->update();

	manager_->flushMessages();
	//El refresh es un método propio de Game State
#endif // !COMPS
}

void MultiplayerState::render()
{
#ifdef COMPS
	manager_->render();
#endif // COMPS
#ifndef COMPS
	renderSys_->update();
#endif // !COMPS
}

bool MultiplayerState::onEnter()
{
	if (SDLNet_Init() < 0) {
		game->exitGame();
		cout << "Conection error" << endl;
	}
	manager_ = new Manager(game);
#ifdef COMPS
	cout << "Using components" << endl;
	manager_->createPlayer();
#endif // COMPS

#ifndef COMPS
	cout << "Using systems" << endl;
	gameCtrlSys_ = manager_->addSystem<GameCtrlSystem>();

	bulletSys_ = manager_->addSystem<BulletsSystem>();

	collisionSys_ = manager_->addSystem<CollisionsSystem>();

	fighterSys_ = manager_->addSystem<FighterSystem>();

	renderSys_ = manager_->addSystem<RenderSystem>();
#endif // !COMPS
	auto& sdl = *SDLUtils::instance();
	Music::setMusicVolume(8); //Musica de fondo y volumen
	sdl.musics().at("theme").play();
	SoundEffect::setChannelVolume(25);
	return true;
}

void MultiplayerState::refresh()
{
	manager_->refresh();
}

void MultiplayerState::resetGame()
{
	//TO DO
}

void MultiplayerState::checkCollisions()
{
	//TO DO
}

MultiplayerState::~MultiplayerState()
{
	Music::haltMusic();
	delete manager_;
	SDLNet_Quit();
}
