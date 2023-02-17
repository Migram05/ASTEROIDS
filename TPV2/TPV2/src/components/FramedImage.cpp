#include "FramedImage.h"

FramedImage::FramedImage(Texture* text, int r, int c) : tex_(text), numRows_(r), numCols_(c), tPerFrame(500)
{
}

FramedImage::FramedImage(Texture* text, int r, int c, int f) : tex_(text), numRows_(r), numCols_(c), tPerFrame(f)
{
}

void FramedImage::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
}

void FramedImage::render()
{
	SDL_Rect dest = build_sdlrect(tr_->getPos(), tr_->getW(), tr_->getH());
	tex_->renderFrame(dest, row, col);
	row = (SDL_GetTicks() / tPerFrame) % numRows_; col = (SDL_GetTicks() / tPerFrame) % numCols_;
}

FramedImage::~FramedImage()
{

}

