#include "BulletsSystem.h"
#include "../ecs/Manager.h"
void BulletsSystem::receive(const Message& m)
{
	switch (m.id)
	{
	case _m_SHOOT: shoot(m.shot_data.pos_, m.shot_data.dir_, m.shot_data.r_); break; //Crea una bala
	case _m_BULLETCOLLIDES: onCollision_BulletAsteroid(m.bulletCollision_data.b_); break; //Destruye la bala si colisiona
	case (_m_PLAYERLOST || _m_PLAYERWINS): onRoundOver(); break; //Destuye las balas
	default: break;
	}
}

void BulletsSystem::initSystem()
{
}

void BulletsSystem::update() //Actualiza todas las balas
{
	for (auto e : mngr_->getEntitiesByGroup(ecs::_grp_BULLETS)) {
		auto tr_ = mngr_->getComponent<Transform>(e);
		auto& position = tr_->getPos(); 
		auto velocity = tr_->getVel();
		position = position + velocity;
		//Si sale de la pantalla, se desactiva
		if (position.getX() + tr_->getW() < 0 || position.getX() > mngr_->getWidth() || position.getY() + tr_->getH() < 0 || position.getY() > mngr_->getHeight()) mngr_->setAlive(e, false);
	}
}

void BulletsSystem::shoot(Vector2D pos, Vector2D dir, float rot) //Crea una bala
{
	mngr_->spawnShot(pos, dir, rot);
}

void BulletsSystem::onCollision_BulletAsteroid(Entity* b) //Al colisionar desactiva la bala
{
	mngr_->setAlive(b, false);
}

void BulletsSystem::onRoundOver() //Desactiva todas las balas al final de la partida
{
	for (auto e : mngr_->getEntitiesByGroup(ecs::_grp_BULLETS)) mngr_->setAlive(e, false);
}

void BulletsSystem::onRoundStart()
{
}
