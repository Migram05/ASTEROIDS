#include "Gun.h"

Gun::Gun() : speed (1), shootRate(300)
{
	lastShotTime = -shootRate;
}

Gun::Gun(float s) : speed(s), shootRate(300)
{
	lastShotTime = -shootRate;
}

Gun::Gun(float s, int shootR) : speed(s), shootRate(shootR)
{
	lastShotTime = -shootRate;
}

void Gun::initComponent()
{

}

void Gun::shoot(Vector2D pos, Vector2D dir, float rot)
{
	auto& sdl = *SDLUtils::instance();
	if (sdl.currRealTime() - lastShotTime >= shootRate) {
		sdl.soundEffects().at("shot").play();
		mngr_->spawnShot(pos, Vector2D{ dir.getX() * speed, dir.getY() * -speed }, rot);
		lastShotTime = SDL_GetTicks();
	}
}

Gun::~Gun()
{
}