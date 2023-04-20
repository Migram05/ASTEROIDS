#include "CollisionsSystem.h"
#include "../ecs/Manager.h"
void CollisionsSystem::receive(const Message& m) //No recibe mensajes, solo env�a
{
}

void CollisionsSystem::initSystem()
{
	isMultiplayer = (mngr_->getGame()->getState()->getStateID() == "MULTIPLAYER");
}

void CollisionsSystem::update() //Comprueba colisiones
{
	vector<Entity*> asteroids = mngr_->getEntitiesByGroup(ecs::_grp_ASTEROIDS); //Se guardan las distintas entidades
	vector<Entity*> bullets = mngr_->getEntitiesByGroup(ecs::_grp_BULLETS);
	vector<Entity*> player = mngr_->getEntitiesByGroup(ecs::_grp_PLAYER);
	auto& sdl = *SDLUtils::instance();
	if (isMultiplayer) //Si hay m�s de un jugador las colisiones ser�n ajustadas para el multijugador
	{
		for (Entity* e : player) {
			Transform* t1 = mngr_->getComponent<Transform>(e);
			for (Entity* b : bullets) { //Primero comprueba sus colisiones con las balas
				Transform* t2 = mngr_->getComponent<Transform>(b);
				if ( mngr_->isAlive(b) && Collisions::collidesWithRotation(t1->getPos(), t1->getW(), t1->getH(), t1->getRotation(), t2->getPos(), t2->getW(), t2->getH(), t2->getRotation())) {
					cout << "colision nave/bala" << endl;
					sdl.soundEffects().at("explosion").play();
					mngr_->setAlive(b, false);
					int& lives = mngr_->getComponent<Health>(e)->getLives();
					lives--;
					Message msg;
					if (lives <= 0) {
						msg.id = _m_EXIT;
						//msg.mainMenuInfo.menuInfoData = "DERROTA";
					}
					else {
						msg.id = _m_RESETPLAYERS; //Colisi�n jugador/asteroide, fin de la partida
					}
					
					mngr_->send(msg);
				}
			}
			for (Entity* p : player) { //Comprueba la colisi�n con el jugador
				Transform* t3 = mngr_->getComponent<Transform>(p);//Si colisiona
				if (e!=p && Collisions::collidesWithRotation(t1->getPos(), t1->getW(), t1->getH(), t1->getRotation(), t3->getPos(), t3->getW(), t3->getH(), t3->getRotation())) {
					cout << "colision nave/nave" << endl;
					sdl.soundEffects().at("explosion").play();
					int& lives = mngr_->getComponent<Health>(e)->getLives();
					lives--;
					int& lives2 = mngr_->getComponent<Health>(p)->getLives();
					lives2--;
					Message msg; 
					if (lives <= 0 || lives2 <= 0) {
						msg.id = _m_EXIT;
						//if(lives <= 0) msg.mainMenuInfo.menuInfoData = "DERROTA";
						//else msg.mainMenuInfo.menuInfoData = "VICTORIA";
					}
					else {
						msg.id = _m_RESETPLAYERS; //Colisi�n jugador/asteroide, fin de la partida
					}
					mngr_->send(msg);
				}
			}
		}
	}
	else {
		for (Entity* a : asteroids)
		{
			Transform* t1 = mngr_->getComponent<Transform>(a);
			for (Entity* b : bullets) { //Primero comprueba sus colisiones con las balas
				Transform* t2 = mngr_->getComponent<Transform>(b);
				if (mngr_->isAlive(b) && Collisions::collidesWithRotation(t1->getPos(), t1->getW(), t1->getH(), t1->getRotation(), t2->getPos(), t2->getW(), t2->getH(), t2->getRotation())) {
					sdl.soundEffects().at("bang").play(); //Efecto de sonido
					Message msg; msg.id = _m_BULLETCOLLIDES; msg.bulletCollision_data.b_ = b; msg.bulletCollision_data.a_ = a; //Mensaje de colisi�n bala/asteroide
					mngr_->send(msg);
				}
			}
			for (Entity* p : player) { //Comprueba la colisi�n con el jugador
				Transform* t3 = mngr_->getComponent<Transform>(p);//Si colisiona
				if (Collisions::collidesWithRotation(t1->getPos(), t1->getW(), t1->getH(), t1->getRotation(), t3->getPos(), t3->getW(), t3->getH(), t3->getRotation())) {
					sdl.soundEffects().at("explosion").play();
					int& lives = mngr_->getComponent<Health>(p)->getLives();
					lives--;
					if (lives <= 0) lives = mngr_->getComponent<Health>(p)->getInitialLives();
					Message msg; msg.id = _m_PLAYERLOST; //Colisi�n jugador/asteroide, fin de la partida
					mngr_->send(msg);
				}
			}
		}
	}
	
}

