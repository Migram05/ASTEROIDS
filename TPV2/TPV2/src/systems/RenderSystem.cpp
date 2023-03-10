#include "RenderSystem.h"
#include "../ecs/Manager.h"
void RenderSystem::receive(const Message& m)
{
}

void RenderSystem::initSystem()
{
}

void RenderSystem::update()
{
	for (auto e : mngr_->getEntitiesByGroup(ecs::_grp_ASTEROIDS)) {
		auto tr_ = mngr_->getComponent<Transform>(e);
		auto fImg_ = mngr_->getComponent<FramedImage>(e);
		auto tex_ = fImg_->getTexture();
		SDL_Rect dest = build_sdlrect(tr_->getPos(), tr_->getW(), tr_->getH()); //Crea el rectángulo de destino
		auto& row = fImg_->getRow();
		auto& col = fImg_->getCol();
		auto numRows_ = fImg_->getNRows(); auto numCols_ = fImg_->getNCols();
		auto tPerFrame = fImg_->getTPerFrame();
		tex_->renderFrame(dest, row, col); //Render del frame
		//Cambio de la columna y fila según los ticks y tiempo entre frames
		row = (SDL_GetTicks() / tPerFrame) % numRows_; col = (SDL_GetTicks() / tPerFrame) % numCols_;
	}
	for (auto e : mngr_->getEntitiesByGroup(ecs::_grp_PLAYER)) {
		auto tr_ = mngr_->getComponent<Transform>(e);
		auto tex_ = mngr_->getComponent<Image>(e)->getTexture();
		SDL_Rect dest = build_sdlrect(tr_->getPos(), tr_->getW(), tr_->getH()); //Crea el rectángulo destino
		tex_->render(dest, tr_->getRotation()); //Renderiza la textura
	}
	for (auto e : mngr_->getEntitiesByGroup(ecs::_grp_BULLETS)) {
		auto tr_ = mngr_->getComponent<Transform>(e);
		auto tex_ = mngr_->getComponent<Image>(e)->getTexture();
		SDL_Rect dest = build_sdlrect(tr_->getPos(), tr_->getW(), tr_->getH()); //Crea el rectángulo destino
		tex_->render(dest, tr_->getRotation()); //Renderiza la textura
	}
	Health* playerHealth =  mngr_->getComponent<Health>(mngr_->getPlayer());
	int nLives = playerHealth->getLives();
	int hWidth = playerHealth->getWidth(), hHeight = playerHealth->getHeight();
	Texture* tex_ = playerHealth->getTexture();
	for (int i = 0; i < nLives; ++i) {
		SDL_Rect dest = build_sdlrect(hWidth * i, mngr_->getHeight() / 90, hWidth, hHeight);
		tex_->render(dest);
	}
}

void RenderSystem::onRoundStart()
{
}

void RenderSystem::onRoundOver()
{
}

void RenderSystem::onGameStart()
{
}

void RenderSystem::onGameOver()
{
}
