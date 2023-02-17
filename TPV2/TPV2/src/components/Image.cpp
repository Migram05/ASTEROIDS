#include "Image.h"

Image::Image(Texture* text) : tr_(nullptr), tex_(text), Component()
{
}

void Image::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
}

void Image::render()
{ 
	SDL_Rect dest = build_sdlrect(tr_->getPos(), tr_->getW(), tr_->getH());
	tex_->render(dest, tr_->getRotation());
}

Image::~Image()
{
}
