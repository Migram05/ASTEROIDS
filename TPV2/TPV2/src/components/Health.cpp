#include "Health.h"

Health::Health(Texture* text, int w, int h) : tex_(text), numLives(3), initialLives(3), Totwidth_(w), Totheight_(h)
{
	width_ = height_ = Totwidth_ / proportion;
}

Health::Health(Texture* text,int w, int h, int n) : tex_(text), numLives(n), initialLives(n), Totwidth_(w), Totheight_(h)
{
	width_ = height_ = Totwidth_ / proportion;
}

void Health::render()
{
	for (int i = 0; i < numLives; ++i) {
		SDL_Rect dest = build_sdlrect( width_ * i, Totheight_ /90, width_, height_);
		tex_->render(dest);
	}
}

int Health::getLives()
{
	return numLives;
}

void Health::damage()
{
	--numLives;
}

void Health::resetLives()
{
	numLives = initialLives;
}
Health::~Health()
{
}