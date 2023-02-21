#include "Image.h"

Image::Image(Texture* text) : tr_(nullptr), tex_(text), Component() //Constructora
{
}

void Image::initComponent()
{
	tr_ = ent_->getComponent<Transform>(); //Busca el componente transform
}

void Image::render()
{ 
	SDL_Rect dest = build_sdlrect(tr_->getPos(), tr_->getW(), tr_->getH()); //Crea el rectángulo destino
	tex_->render(dest, tr_->getRotation()); //Renderiza la textura
}

Image::~Image()
{
}
