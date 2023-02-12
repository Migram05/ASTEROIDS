#include "Gun.h"

Gun::Gun() : speed (1), shootRate(1000)
{
	lastShotTime = -shootRate;
}

Gun::Gun(float s) : speed(s), shootRate(1000)
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

void Gun::shoot(Vector2D pos, Vector2D dir)
{
	if (SDL_GetTicks() - lastShotTime >= shootRate) {
		cout << "Disparo" << endl;
		lastShotTime = SDL_GetTicks();
	}
}

Gun::~Gun()
{
}
