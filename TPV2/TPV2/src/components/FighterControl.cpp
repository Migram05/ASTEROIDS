#include "FighterControl.h"
#include "../ecs/Manager.h"
FighterControl::FighterControl() : speed(1.2), rotationSpeed(5) //Constructora por defecto
{
}

FighterControl::FighterControl(float s, float rS) : speed(s), rotationSpeed(rS) //Constructora por valor
{
}

void FighterControl::initComponent() //Inicializa
{
	tr_ = mngr_->getComponent<Transform>(ent_); //Obtiene el transform

}

void FighterControl::update() 
{
	Vector2D& fVector = tr_->getVel(); //Obtiene velocidad
	float& fRotation = tr_->getRotation(); //Rotacion del caza
	Vector2D& forwardVector = tr_->getForward(); //Vector forward actual
	Vector2D& lastForward = tr_->getLastForward(); //Ultimo vector forward
	double rad = (fRotation) * (M_PI / 180);
	float c = cos(rad); 
	float s = sin(rad); 
	forwardVector = Vector2D{ s, c }; //Se actualiza el vector forward según su rotación
	SDL_Event event;
	auto& sdl = *SDLUtils::instance();
	while (SDL_PollEvent(&event)) { //Controla la entrada
		if (event.type == SDL_KEYDOWN) {
			switch (event.key.keysym.sym)
			{
			case SDLK_w: {lastForward = forwardVector; fVector = Vector2D{ lastForward.getX() * speed, lastForward.getY() * -speed }; sdl.soundEffects().at("thrust").play(); break; } //Avanza
			case SDLK_a: fRotation -= rotationSpeed; if (fRotation < -360) fRotation = 0; break; //Rotación
			case SDLK_d: fRotation += rotationSpeed; if (fRotation > 360) fRotation = 0; break;
			case SDLK_s: { //Dispara las armas
				if (mngr_->hasComponent<Gun>(ent_)) {
					mngr_->getComponent<Gun>(ent_)->shoot(tr_->getPos(), tr_->getForward(), tr_->getRotation());
				}
				break;
			}
			case SDLK_ESCAPE: mngr_->exitGame(); break; //(Adicional) Sale del juego
			case SDLK_SPACE: mngr_->pauseGame(); break;
			default:
				break;
			}
			
		}
	}

}

FighterControl::~FighterControl() //Destructora
{
}
