#include "CollisionsSystem.h"
#include "../ecs/Manager.h"
void CollisionsSystem::receive(const Message& m)
{
}

void CollisionsSystem::initSystem()
{
}

void CollisionsSystem::update()
{
	vector<Entity*> asteroids = mngr_->getEntitiesByGroup(ecs::_grp_ASTEROIDS); //Se guardan las distintas entidades
	vector<Entity*> bullets = mngr_->getEntitiesByGroup(ecs::_grp_BULLETS);
	vector<Entity*> player = mngr_->getEntitiesByGroup(ecs::_grp_PLAYER);
	bool reset = false;
	auto& sdl = *SDLUtils::instance();
	auto it = asteroids.begin();
	while (it != asteroids.end() && !reset) { //Se recorren los asteroides
		Entity* a = *it;
		Transform* t1 = mngr_->getComponent<Transform>(a);
		for (Entity* b : bullets) { //Primero comprueba sus colisiones con las balas
			Transform* t2 = mngr_->getComponent<Transform>(b);
			if (mngr_->isAlive(b) && Collisions::collidesWithRotation(t1->getPos(), t1->getW(), t1->getH(), t1->getRotation(), t2->getPos(), t2->getW(), t2->getH(), t2->getRotation())) {
				sdl.soundEffects().at("bang").play(); //Efecto de sonido
				Message msg; msg.id = _m_BULLETCOLLIDES; msg.bulletCollision_data.b_ = b; msg.bulletCollision_data.a_ = a;
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
				Message msg; msg.id = _m_PLAYERLOST;
				mngr_->send(msg);
				//t3->getPos() = Vector2D{ (float)WIN_WIDTH / 2, (float)WIN_HEIGHT / 2 }; t3->getRotation() = 0; t3->getVel() = Vector2D{ 0,0 };
				//asteroidsManager_->destroyAllAsteroids(); // Se borran los asteroides
				//reset = true; //Variable que termina el bucle y resetea el juego
			}
		}
		++it;
	}
	//if (reset) resetGame(); //Reset del juego
}

void CollisionsSystem::onRoundOver()
{
}

void CollisionsSystem::onRoundStart()
{
}
