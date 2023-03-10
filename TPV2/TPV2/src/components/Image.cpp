#include "Image.h"
#include "../ecs/Manager.h"
Image::Image(Texture* text) : tr_(nullptr), tex_(text), Component() //Constructora
{
}

void Image::initComponent()
{
	tr_ = mngr_->getComponent<Transform>(ent_); //Busca el componente transform
}


Image::~Image()
{
}

#ifdef COMPS
void Image::render()
{
	SDL_Rect dest = build_sdlrect(tr_->getPos(), tr_->getW(), tr_->getH()); //Crea el rectángulo destino
	tex_->render(dest, tr_->getRotation()); //Renderiza la textura
}
#endif // COMPS

