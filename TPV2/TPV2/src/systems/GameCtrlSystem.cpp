#include "GameCtrlSystem.h"
#include "../ecs/Manager.h"
void GameCtrlSystem::receive(const Message& m)
{
	switch (m.id)
	{
	case _m_PAUSEGAME: pauseGame(); break; //Mensajes para controlar la máquina de estados
	case _m_PLAYERLOST: onCollision_FighterAsteroid(); break;
	case _m_PLAYERWINS: onAsteroidsExtinction();  break;
	case _m_EXIT: exitGame(); break;
	default: break;
	}
}

void GameCtrlSystem::initSystem() //Al crear envía un mensaje de comienzo de partida
{
	Message msg;
	msg.id = _m_NEWGAME;
	mngr_->send(msg, true);
}

void GameCtrlSystem::update()
{
}

void GameCtrlSystem::onCollision_FighterAsteroid() //Menú de pausa derrota
{
	mngr_->playerLost();
}

void GameCtrlSystem::onAsteroidsExtinction()//Menú de pausa victoria
{
	mngr_->playerLost();
}

void GameCtrlSystem::pauseGame() //Menú de pausa
{
	mngr_->pauseGame();
}

void GameCtrlSystem::exitGame() //Sale del juego
{
	mngr_->exitGame();
}
