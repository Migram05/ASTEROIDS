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
		if (e->isVisible()) {
			auto tr_ = mngr_->getComponent<Transform>(e);
			auto fImg_ = mngr_->getComponent<FramedImage>(e);
			tex_ = mngr_->getTexture(GrayAsteroid);
			if (mngr_->hasComponent<Follow>(e)) tex_ = mngr_->getTexture(GoldAsteroid);
			SDL_Rect dest = build_sdlrect(tr_->getPos(), tr_->getW(), tr_->getH()); //Crea el rectángulo de destino
			auto& row = fImg_->getRow();
			auto& col = fImg_->getCol();
			auto numRows_ = fImg_->getNRows(); auto numCols_ = fImg_->getNCols();
			auto tPerFrame = fImg_->getTPerFrame();
			tex_->renderFrame(dest, row, col); //Render del frame
			//Cambio de la columna y fila según los ticks y tiempo entre frames
			row = (SDL_GetTicks() / tPerFrame) % numRows_; col = (SDL_GetTicks() / tPerFrame) % numCols_;
		}
	}
	tex_ = mngr_->getTexture(Fighter1);
	hTex_ = mngr_->getTexture(Heart);
	int nP = 0;
	for (auto e : mngr_->getEntitiesByGroup(ecs::_grp_PLAYER)) {
		if (e->isVisible()) {
			auto tr_ = mngr_->getComponent<Transform>(e);
			SDL_Rect dest = build_sdlrect(tr_->getPos(), tr_->getW(), tr_->getH()); //Crea el rectángulo destino
			tex_->render(dest, tr_->getRotation()); //Renderiza la textura
		}
		Health* playerHealth = mngr_->getComponent<Health>(mngr_->getPlayer(nP));
		int nLives = playerHealth->getLives();
		int hWidth = playerHealth->getWidth(), hHeight = playerHealth->getHeight();

		for (int i = 0; i < nLives; ++i) {
			SDL_Rect dest = build_sdlrect(hWidth * i, (mngr_->getHeight() / 90) + hHeight * nP, hWidth, hHeight);
			hTex_->render(dest);
		}
		nP++;
	}
	tex_ = mngr_->getTexture(Fire);
	for (auto e : mngr_->getEntitiesByGroup(ecs::_grp_BULLETS)) {
		if (e->isVisible()) {
			auto tr_ = mngr_->getComponent<Transform>(e);
			SDL_Rect dest = build_sdlrect(tr_->getPos(), tr_->getW(), tr_->getH()); //Crea el rectángulo destino
			tex_->render(dest, tr_->getRotation()); //Renderiza la textura
		}
	}
	
	for (auto e : mngr_->getEntitiesByGroup(ecs::_grp_UI)) {
		if (e->isVisible()) {
			tex_ = mngr_->getComponent<Image>(e)->getTexture();
			auto tr_ = mngr_->getComponent<Transform>(e);
			SDL_Rect dest = build_sdlrect(tr_->getPos(), tr_->getW(), tr_->getH()); //Crea el rectángulo destino
			tex_->render(dest, tr_->getRotation()); //Renderiza la textura
		}
	}
}

