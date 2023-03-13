#include "GameCtrlSystem.h"
#include "../ecs/Manager.h"
void GameCtrlSystem::receive(const Message& m)
{
	switch (m.id)
	{
	case _m_PAUSEGAME: pauseGame(); break; //Mensajes para controlar la m�quina de estados
	case _m_PLAYERLOST: onCollision_FighterAsteroid(); break;
	case _m_PLAYERWINS: onAsteroidsExtinction();  break;
	case _m_EXIT: exitGame(); break;
	default: break;
	}
}

void GameCtrlSystem::initSystem() //Al crear env�a un mensaje de comienzo de partida
{
	Message msg;
	msg.id = _m_NEWGAME;
	mngr_->send(msg, true);
}

void GameCtrlSystem::update()
{
}

void GameCtrlSystem::onCollision_FighterAsteroid() //Men� de pausa derrota
{
	mngr_->playerLost();
}

void GameCtrlSystem::onAsteroidsExtinction()//Men� de pausa victoria
{
	mngr_->playerLost();
}

void GameCtrlSystem::pauseGame() //Men� de pausa
{
	mngr_->pauseGame();
}

void GameCtrlSystem::exitGame() //Sale del juego
{
	mngr_->exitGame();
}
