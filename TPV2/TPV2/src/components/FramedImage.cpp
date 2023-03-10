#include "FramedImage.h"
#include "../ecs/Manager.h"
FramedImage::FramedImage(Texture* text, int r, int c) : tex_(text), numRows_(r), numCols_(c), tPerFrame(500) 
{
	//Constructora por defecto
}

FramedImage::FramedImage(Texture* text, int r, int c, int f) : tex_(text), numRows_(r), numCols_(c), tPerFrame(f)
{
	//Constructora con valor para el tiempo entre frames
}

void FramedImage::initComponent()
{
	tr_ = mngr_->getComponent<Transform>(ent_); //Inicializa el componente, buscando el componente transform
}

FramedImage::~FramedImage() //Destructora
{

}
#ifdef COMPS
void FramedImage::render() //Render de la imagen con frames
{
	SDL_Rect dest = build_sdlrect(tr_->getPos(), tr_->getW(), tr_->getH()); //Crea el rectángulo de destino
	tex_->renderFrame(dest, row, col); //Render del frame
	//Cambio de la columna y fila según los ticks y tiempo entre frames
	row = (SDL_GetTicks() / tPerFrame) % numRows_; col = (SDL_GetTicks() / tPerFrame) % numCols_;
}
#endif // COMPS


