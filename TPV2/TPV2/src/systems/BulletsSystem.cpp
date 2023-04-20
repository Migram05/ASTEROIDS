#include "BulletsSystem.h"
#include "../ecs/Manager.h"
void BulletsSystem::receive(const Message& m)
{
	switch (m.id)
	{
	case _m_SHOOT: shoot(m.shot_data.pos_, m.shot_data.dir_, m.shot_data.r_); break; //Crea una bala
	case _m_BULLETCOLLIDES: onCollision_BulletAsteroid(m.bulletCollision_data.b_); break; //Destruye la bala si colisiona
	case (_m_PLAYERLOST || _m_PLAYERWINS): onRoundOver(); break; //Destuye las balas
	case _m_SHIPSHOOT: spawnShotAtPlayer(m.shipShoot_data.indx); break;
	case _m_SHIPSTATE: {
		if (m.shipData.S) {
			spawnShotAtPlayer(m.shipData.idx); break;
		}
	}
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

void BulletsSystem::spawnShotAtPlayer(int index) //Crea un disparo en el jugador que lo solicita
{
	auto shootP = mngr_->getPlayer(index);
	auto tr_ = mngr_->getComponent<Transform>(shootP);
	auto gun_ = mngr_->getComponent<Gun>(shootP);
	auto& position_ = tr_->getPos();
	float& fRotation = tr_->getRotation(); //Rotacion del caza
	Vector2D& forwardVector = tr_->getForward(); //Vector forward actual
	double rad = (fRotation) * (M_PI / 180);
	float c = cos(rad), s = sin(rad);
	forwardVector = Vector2D{ s, c }; //Se actualiza el vector forward según su rotación
	Vector2D dir = Vector2D{ forwardVector.getX() * gun_->getSpeed(), forwardVector.getY() * -gun_->getSpeed() }; 
	mngr_->spawnShot(position_ + (dir * 4), dir, fRotation);
}

