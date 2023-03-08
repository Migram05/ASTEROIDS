#include "GameCtrlSystem.h"
#include "../ecs/Manager.h"
void GameCtrlSystem::receive(const Message& m)
{
	switch (m.id)
	{
	case _m_PAUSEGAME: pauseGame(); break;
	case _m_PLAYERLOST: onCollision_FighterAsteroid(); break;
	case _m_PLAYERWINS: onAsteroidsExtinction();  break;
	case _m_EXIT: exitGame(); break;
	default: break;
	}
}

void GameCtrlSystem::initSystem()
{
	Message msg;
	msg.id = _m_NEWGAME;
	mngr_->send(msg);
	mngr_->createPlayer();
}

void GameCtrlSystem::update()
{
}

void GameCtrlSystem::onCollision_FighterAsteroid()
{
	mngr_->playerLost();
}

void GameCtrlSystem::onAsteroidsExtinction()
{
	mngr_->playerLost();
}

void GameCtrlSystem::pauseGame()
{
	mngr_->pauseGame();
}

void GameCtrlSystem::exitGame() //Sale del juego
{
	mngr_->exitGame();
}
