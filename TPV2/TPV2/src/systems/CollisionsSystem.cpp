#include "CollisionsSystem.h"
#include "../ecs/Manager.h"
#include "../states/MultiplayerState.h"
void CollisionsSystem::receive(const Message& m) //No recibe mensajes, solo envía
{
}

void CollisionsSystem::initSystem()
{
	isMultiplayer = (mngr_->getGame()->getState()->getStateID() == "MULTIPLAYER");
	if(isMultiplayer) multiState = static_cast<MultiplayerState*>(mngr_->getGame()->getState());
}

void CollisionsSystem::update() //Comprueba colisiones
{

	if (isMultiplayer) //Si hay más de un jugador las colisiones serán ajustadas para el multijugador
	{
		multiPlayerCollisions();
	}
	else {
		singlePlayerCollisions();
	}
	
}

void CollisionsSystem::multiPlayerCollisions()
{
	vector<Entity*> bullets = mngr_->getEntitiesByGroup(ecs::_grp_BULLETS);
	vector<Entity*> player = mngr_->getEntitiesByGroup(ecs::_grp_PLAYER);
	auto& sdl = *SDLUtils::instance();
	for (Entity* e : player) {
		Transform* t1 = mngr_->getComponent<Transform>(e);
		for (Entity* b : bullets) { //Primero comprueba sus colisiones con las balas
			Transform* t2 = mngr_->getComponent<Transform>(b);
			if (mngr_->isAlive(b) && Collisions::collidesWithRotation(t1->getPos(), t1->getW(), t1->getH(), t1->getRotation(), t2->getPos(), t2->getW(), t2->getH(), t2->getRotation())) {
				sdl.soundEffects().at("explosion").play(); //Efecto
				mngr_->setAlive(b, false);
				int& lives = mngr_->getComponent<Health>(e)->getLives();
				lives--;
				Message msg;
				if (lives <= 0) { //Si un jugador se queda sin vidas, se termina el juego
					msg.id = _m_EXIT;
				}
				else {
					msg.id = _m_RESETPLAYERS; //En caso de haber más vidas, se reinician las posiciones
				}
				mngr_->send(msg);
			}
		}
		for (Entity* p : player) { //Comprueba la colisión con el enemigo
			Transform* t3 = mngr_->getComponent<Transform>(p);//Si colisiona
			if (e != p && Collisions::collidesWithRotation(t1->getPos(), t1->getW(), t1->getH(), t1->getRotation(), t3->getPos(), t3->getW(), t3->getH(), t3->getRotation())) {
				sdl.soundEffects().at("explosion").play();
				int& lives = mngr_->getComponent<Health>(e)->getLives(); lives--;
				int& lives2 = mngr_->getComponent<Health>(p)->getLives(); lives2--;
				Message msg;
				if (lives <= 0 || lives2 <= 0) { //Si alguno se queda sin vidas, salen al menú
					msg.id = _m_EXIT;
				}
				else {
					msg.id = _m_RESETPLAYERS; //En caso de haber más vidas, se resetean las posiciones
				}
				mngr_->send(msg);
			}
		}
	}
}

void CollisionsSystem::singlePlayerCollisions()
{
	vector<Entity*> asteroids = mngr_->getEntitiesByGroup(ecs::_grp_ASTEROIDS); //Se guardan las distintas entidades
	vector<Entity*> bullets = mngr_->getEntitiesByGroup(ecs::_grp_BULLETS);
	vector<Entity*> player = mngr_->getEntitiesByGroup(ecs::_grp_PLAYER);
	auto& sdl = *SDLUtils::instance();
	for (Entity* a : asteroids)
	{
		Transform* t1 = mngr_->getComponent<Transform>(a);
		for (Entity* b : bullets) { //Primero comprueba sus colisiones con las balas
			Transform* t2 = mngr_->getComponent<Transform>(b);
			if (mngr_->isAlive(b) && Collisions::collidesWithRotation(t1->getPos(), t1->getW(), t1->getH(), t1->getRotation(), t2->getPos(), t2->getW(), t2->getH(), t2->getRotation())) {
				sdl.soundEffects().at("bang").play(); //Efecto de sonido
				Message msg; msg.id = _m_BULLETCOLLIDES; msg.bulletCollision_data.b_ = b; msg.bulletCollision_data.a_ = a; //Mensaje de colisión bala/asteroide
				mngr_->send(msg);
			}
		}
		for (Entity* p : player) { //Comprueba la colisión con el jugador
			Transform* t3 = mngr_->getComponent<Transform>(p);//Si colisiona
			if (Collisions::collidesWithRotation(t1->getPos(), t1->getW(), t1->getH(), t1->getRotation(), t3->getPos(), t3->getW(), t3->getH(), t3->getRotation())) {
				sdl.soundEffects().at("explosion").play();
				int& lives = mngr_->getComponent<Health>(p)->getLives();
				lives--;
				if (lives <= 0) lives = mngr_->getComponent<Health>(p)->getInitialLives();
				Message msg; msg.id = _m_PLAYERLOST; //Colisión jugador/asteroide, fin de la partida
				mngr_->send(msg);
			}
		}
	}
}

