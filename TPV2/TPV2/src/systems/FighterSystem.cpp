#include "FighterSystem.h"
#include "../ecs/Manager.h"
void FighterSystem::receive(const Message& m)
{
	switch (m.id)
	{
	case _m_PLAYERLOST: onCollision_FighterAsteroid(); break;
	default: break;
	}
}

void FighterSystem::initSystem()
{
	mngr_->createPlayer();
}

void FighterSystem::update()
{
	Entity* p = mngr_->getPlayer();
	auto tr_ = mngr_->getComponent<Transform>(p);
	auto gun_ = mngr_->getComponent<Gun>(p);
	auto& position_ = tr_->getPos();
	Vector2D& v = tr_->getVel(); //Obtiene velocidad
	float& fRotation = tr_->getRotation(); //Rotacion del caza
	Vector2D& forwardVector = tr_->getForward(); //Vector forward actual
	Vector2D& lastForward = tr_->getLastForward(); //Ultimo vector forward
	double rad = (fRotation) * (M_PI / 180);
	float c = cos(rad);
	float s = sin(rad);
	forwardVector = Vector2D{ s, c }; //Se actualiza el vector forward seg�n su rotaci�n
	SDL_Event event;
	auto& sdl = *SDLUtils::instance();
	Message msg;
	while (SDL_PollEvent(&event)) { //Controla la entrada
		if (event.type == SDL_KEYDOWN) {
			switch (event.key.keysym.sym)
			{
				case SDLK_w: {lastForward = forwardVector; v = Vector2D{ lastForward.getX() * speed, lastForward.getY() * -speed }; sdl.soundEffects().at("thrust").play(); break; } //Avanza
				case SDLK_a: fRotation -= rotationSpeed; if (fRotation < -360) fRotation = 0; break; //Rotaci�n
				case SDLK_d: fRotation += rotationSpeed; if (fRotation > 360) fRotation = 0; break;
				case SDLK_s: { //Dispara las armas
					auto& sdl = *SDLUtils::instance();
					if (sdl.currRealTime() - gun_->getLastShotTime() >= gun_->getShootRate()) { //Se comprueba la cadencia
						sdl.soundEffects().at("shot").play(); //Efecto de sonido
						gun_->setLastShoot(SDL_GetTicks());
						msg.id = _m_SHOOT; msg.shot_data.pos_ = position_; 
						msg.shot_data.dir_ = Vector2D { forwardVector.getX() * gun_->getSpeed(), forwardVector.getY() * -gun_->getSpeed()};
						msg.shot_data.r_ = fRotation;
						mngr_->send(msg);
					}
					break;
				}
				case SDLK_ESCAPE: {
					msg.id = _m_EXIT;
					mngr_->send(msg);
					break;
				}
				case SDLK_SPACE: {
					msg.id = _m_PAUSEGAME;
					mngr_->send(msg);
					break;
				}

				default: break;
			}

		}
	}
	position_ = position_ + v;
	auto dAcceel_ = mngr_->getComponent<DeAcceleration>(p);
	auto stopMargin = dAcceel_->getStopMargin();
	auto reduction = dAcceel_->getReduction();
	if (abs(v.getX()) > stopMargin || abs(v.getY()) > stopMargin) //En caso de que la velocidad sea superior al margen se reduce
		v = v + Vector2D{ lastForward.getX() * -reduction, lastForward.getY() * reduction };
	else v = { 0,0 }; //En caso de ser menor al margen, se detiene la nave
}

void FighterSystem::onCollision_FighterAsteroid()
{
	Entity* p = mngr_->getPlayer();
	auto tr_ = mngr_->getComponent<Transform>(p);
	auto& pos = tr_->getPos(); auto& vel = tr_->getVel();
	pos = Vector2D(mngr_->getWidth() / 2, mngr_->getHeight() / 2);
	vel = Vector2D(0, 0);
}

void FighterSystem::onRoundOver()
{
}

void FighterSystem::onRoundStart()
{
}
