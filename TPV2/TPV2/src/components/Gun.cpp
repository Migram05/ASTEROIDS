#include "Gun.h"

Gun::Gun() : speed (1), shootRate(100)
{
	lastShotTime = -shootRate;
}

Gun::Gun(float s) : speed(s), shootRate(100)
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
	if (SDL_GetTicks() - lastShotTime >= shootRate) {
		
		mngr_->spawnShot(pos, Vector2D{ dir.getX() * speed, dir.getY() * -speed }, rot);
		lastShotTime = SDL_GetTicks();
	}
}

Gun::~Gun()
{
}