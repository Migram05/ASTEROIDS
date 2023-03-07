#include "Gun.h"
#include "../ecs/Manager.h"
Gun::Gun() : speed (1), shootRate(300) //Constructora por defecto
{
	lastShotTime = -shootRate; //Se ajusta el tiempo de la última vez disparado para poder disparar nada más entrar
}

Gun::Gun(float s) : speed(s), shootRate(300) //Constructora por valor
{
	lastShotTime = -shootRate;
}

Gun::Gun(float s, int shootR) : speed(s), shootRate(shootR) //Constructora por valor
{
	lastShotTime = -shootRate;
}

void Gun::initComponent()
{

}

void Gun::shoot(Vector2D pos, Vector2D dir, float rot) //Método de disparo (llamada desde el componente de control)
{
	auto& sdl = *SDLUtils::instance(); 
	if (sdl.currRealTime() - lastShotTime >= shootRate) { //Se comprueba la cadencia
		sdl.soundEffects().at("shot").play(); //Efecto de sonido
		mngr_->spawnShot(pos, Vector2D{ dir.getX() * speed, dir.getY() * -speed }, rot);
		lastShotTime = SDL_GetTicks();
	}
}

Gun::~Gun()
{
}