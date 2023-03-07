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
