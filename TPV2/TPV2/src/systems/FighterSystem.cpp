#include "FighterSystem.h"
#include "../ecs/Manager.h"
void FighterSystem::receive(const Message& m)
{
	switch (m.id)
	{
	case _m_PLAYERLOST: onCollision_FighterAsteroid(); break; //Ambos eventos colocan al caza en el centro de la pantalla
	case _m_PLAYERWINS: onCollision_FighterAsteroid(); break;
	default: break;
	}
}

void FighterSystem::initSystem() //Al crear el sistema, se crea el jugador
{

	mngr_->createPlayer();
	p = mngr_->getPlayer();
}

void FighterSystem::update() //Afctualiza la posici�n del jugador
{
	updatePosition();
	speedReduction();
	screenPositionCheck();
}
void FighterSystem::updatePosition() //Mueve al caza
{
	auto tr_ = mngr_->getComponent<Transform>(p);
	auto gun_ = mngr_->getComponent<Gun>(p);
	auto& position_ = tr_->getPos();
	Vector2D& v = tr_->getVel(); //Obtiene velocidad
	float maxSpeed = tr_->getMaxSpeed();
	float& fRotation = tr_->getRotation(); //Rotacion del caza
	Vector2D& forwardVector = tr_->getForward(); //Vector forward actual
	double rad = (fRotation) * (M_PI / 180);
	float c = cos(rad), s = sin(rad);
	forwardVector = Vector2D{ s, c }; //Se actualiza el vector forward seg�n su rotaci�n
	Vector2D newVel = v + Vector2D(0, -1).rotate(fRotation) * speed;
	SDL_Event event;
	auto& sdl = *SDLUtils::instance();
	Message msg;
	while (SDL_PollEvent(&event)) { //Controla la entrada
		if (event.type == SDL_KEYDOWN) {
			switch (event.key.keysym.sym)
			{
			case SDLK_w: { if (newVel.magnitude() >= maxSpeed) newVel = newVel.normalize() * maxSpeed; v = newVel; sdl.soundEffects().at("thrust").play(); break; } //Avanza
			case SDLK_a: fRotation -= rotationSpeed; if (fRotation < -360) fRotation = 0; break; //Rotaci�n
			case SDLK_d: fRotation += rotationSpeed; if (fRotation > 360) fRotation = 0; break;
			case SDLK_s: { //Dispara las armas
				auto& sdl = *SDLUtils::instance();
				if (sdl.currRealTime() - gun_->getLastShotTime() >= gun_->getShootRate()) { //Se comprueba la cadencia
					sdl.soundEffects().at("shot").play(); //Efecto de sonido
					gun_->setLastShoot(SDL_GetTicks());
					msg.id = _m_SHOOT; msg.shot_data.pos_ = position_; msg.shot_data.dir_ = Vector2D{ forwardVector.getX() * gun_->getSpeed(), forwardVector.getY() * -gun_->getSpeed() }; msg.shot_data.r_ = fRotation;
					mngr_->send(msg);
				} break;
			}
			case SDLK_ESCAPE: { //mensaje de salida
				msg.id = _m_EXIT;  mngr_->send(msg); break;
			}
			case SDLK_SPACE: { //Mensaje de pausa
				msg.id = _m_PAUSEGAME; mngr_->send(msg); break;
			}
			default: break;
			}
		}
	}
	position_ = position_ + v; //Actualiza la posici�n
}

void FighterSystem::speedReduction() //Reduce la velocidad del caza
{
	auto tr_ = mngr_->getComponent<Transform>(p);
	auto dAcceel_ = mngr_->getComponent<DeAcceleration>(p);
	auto stopMargin = dAcceel_->getStopMargin();
	auto reduction = dAcceel_->getReduction();
	Vector2D& v = tr_->getVel(); //Se obtiene la velocidad
	v = v * reduction; //Se reduce
	if (v.magnitude() <= stopMargin) v = { 0,0 };
}

void FighterSystem::screenPositionCheck() //Movimiento toroidal
{
	auto tr_ = mngr_->getComponent<Transform>(p);
	auto& position_ = tr_->getPos();
	//Movimiento toroidal
	if (position_.getX() + tr_->getW() < 0) position_ = Vector2D{ (float)mngr_->getWidth() , position_.getY() };
	else if (position_.getX() > mngr_->getWidth()) position_ = Vector2D{ 0 , position_.getY() };

	if (position_.getY() + tr_->getH() < 0) position_ = Vector2D{ position_.getX() ,(float)mngr_->getHeight() };
	else if (position_.getY() > mngr_->getHeight()) position_ = Vector2D{ position_.getX() , 0 };
}
void FighterSystem::onCollision_FighterAsteroid() //En caso de colisi�n con un asteroide
{
	Entity* p = mngr_->getPlayer();
	auto tr_ = mngr_->getComponent<Transform>(p);
	auto& pos = tr_->getPos(); auto& vel = tr_->getVel();
	pos = Vector2D(mngr_->getWidth() / 2, mngr_->getHeight() / 2); //Coloca al caza en el centro
	vel = Vector2D(0, 0); //Velocidad nula
}


