#include "Health.h"

Health::Health(Texture* text, int w, int h) : tex_(text), numLives(3), initialLives(3), Totwidth_(w), Totheight_(h)
{
	//Constructora por defecto
	width_ = height_ = Totwidth_ / proportion; //calculo para saber ancho y alto de los corazones
}

Health::Health(Texture* text,int w, int h, int n) : tex_(text), numLives(n), initialLives(n), Totwidth_(w), Totheight_(h)
{
	//Constructora por valores
	width_ = height_ = Totwidth_ / proportion;
}

void Health::render()
{
	for (int i = 0; i < numLives; ++i) { //Se renderizan las vidas
		SDL_Rect dest = build_sdlrect( width_ * i, Totheight_ /90, width_, height_);
		tex_->render(dest);
	}
}

int Health::getLives() //Devuelve las vidas
{
	return numLives;
}

void Health::damage() //Método de daño
{
	--numLives;
}

void Health::resetLives() //Reset de la vidas
{
	numLives = initialLives;
}
Health::~Health() //Destructora
{
}