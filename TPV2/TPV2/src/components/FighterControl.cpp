#include "FighterControl.h"

FighterControl::FighterControl() : speed(0.4), rotationSpeed(5)
{
}

FighterControl::FighterControl(float s, float rS) : speed(s), rotationSpeed(rS)
{
}

FighterControl::~FighterControl()
{
}

void FighterControl::initComponent()
{
	tr_ = ent_->getComponent<Transform>(ecs::_TRANSFORM);
}

void FighterControl::update()
{
	Vector2D& fVector = tr_->getVel();
	float& fRotation = tr_->getRotation();
	Vector2D& forwardVector = tr_->getForward();
	double rad = (fRotation) * (M_PI / 180);
	float c = cos(rad); 
	float s = sin(rad); 
	
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_KEYDOWN) {
			switch (event.key.keysym.sym)
			{
			case SDLK_w: {forwardVector = Vector2D{ s, c }; fVector = Vector2D{ forwardVector.getX() * speed, forwardVector.getY() * -speed}; break; }
			case SDLK_a: fRotation -= rotationSpeed; if (fRotation < -360) fRotation = 0; break;
			case SDLK_d: fRotation += rotationSpeed; if (fRotation > 360) fRotation = 0; break;
			case SDLK_s: {
				if (ent_->hasComponent(ecs::_GUN)) {
					ent_->getComponent<Gun>(ecs::_GUN)->shoot(tr_->getPos(), tr_->getForward());
				}
			}
			default:
				break;
			}
			
		}
	}
}