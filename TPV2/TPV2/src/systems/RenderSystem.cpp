#include "RenderSystem.h"
#include "../ecs/Manager.h"
#include "../sdlutils/SDLUtils.h"
void RenderSystem::receive(const Message& m)
{
	switch (m.id)
	{
	case _m_SHOWALL: showAllEntities(); break;
	default: break;
	}
}

void RenderSystem::initSystem()
{
	isMultiplayer = (mngr_->getGame()->getState()->getStateID() == "MULTIPLAYER");
}

void RenderSystem::update()
{
	auto& sdl = *SDLUtils::instance();
	//Re renderizan los distintos tipos de objetos
	asteroidRender(); 
	playerRender();
	bulletRender();
	UIRender();
}

void RenderSystem::showAllEntities() //Muestra todas las entidades ocultas de la interfaz
{
	for (Entity* e : mngr_->getEntitiesByGroup(ecs::_grp_UI)) {
		if (!e->isVisible()) e->setVisibility(true);
	}
}

void RenderSystem::asteroidRender()
{
	auto& sdl = *SDLUtils::instance();
	for (auto e : mngr_->getEntitiesByGroup(ecs::_grp_ASTEROIDS)) {
		if (e->isVisible()) {
			auto tr_ = mngr_->getComponent<Transform>(e);
			auto fImg_ = mngr_->getComponent<FramedImage>(e);
			//tex_ = &sdlutils().images().at("GrayAsteroids");
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
}

void RenderSystem::playerRender()
{
	auto& sdl = *SDLUtils::instance();
	tex_ = &sdl.images().at("Fighter"); //Se guardan las distintas texturas
	hTex_ = &sdl.images().at("Heart");
	int nP = 0; //Contador del número de jugadores
	for (auto e : mngr_->getEntitiesByGroup(ecs::_grp_PLAYER)) {
		if (e->isVisible()) {
			auto tr_ = mngr_->getComponent<Transform>(e);
			SDL_Rect dest = build_sdlrect(tr_->getPos(), tr_->getW(), tr_->getH()); //Crea el rectángulo destino
			tex_->render(dest, tr_->getRotation()); //Renderiza la textura
			if (isMultiplayer) {
				//Nombre de los jugadores
				string name;
				if (nP == mngr_->getPlayerIndex()) name = mngr_->getPlayerName(); //Si el índice coincide, escribe el nombre del jugador
				else name = mngr_->getEnemyName(); //Sino el del enemigo
				if (name.empty()) name = "PLAYER " + to_string(nP); //En caso de no haber nombre (jugador no conectado) 
				int tam = name.size() / 2;
				Vector2D texturePos = tr_->getPos() + Vector2D(-tam * 5, tr_->getH());
				Texture lostText(sdl.renderer(), name, sdl.fonts().at("CAPTURE10"), build_sdlcolor(0xffffffff));
				lostText.render(texturePos.getX(), texturePos.getY());
			}
		}
		//Vida de los jugadores
		Health* playerHealth = mngr_->getComponent<Health>(mngr_->getPlayer(nP));
		int nLives = playerHealth->getLives();
		int hWidth = playerHealth->getWidth(), hHeight = playerHealth->getHeight();
		for (int i = 0; i < nLives; ++i) {
			SDL_Rect dest = build_sdlrect(hWidth * i, (mngr_->getHeight() / 90) + hHeight * nP, hWidth, hHeight);
			hTex_->render(dest);
		}
		nP++;
	}
}

void RenderSystem::bulletRender()
{
	auto& sdl = *SDLUtils::instance();
	tex_ = &sdl.images().at("Fire");
	for (auto e : mngr_->getEntitiesByGroup(ecs::_grp_BULLETS)) { //Dibujado de las balas
		if (e->isVisible()) {
			auto tr_ = mngr_->getComponent<Transform>(e);
			SDL_Rect dest = build_sdlrect(tr_->getPos(), tr_->getW(), tr_->getH()); //Crea el rectángulo destino
			tex_->render(dest, tr_->getRotation()); //Renderiza la textura
		}
	}
}

void RenderSystem::UIRender()
{
	auto& sdl = *SDLUtils::instance();
	for (auto e : mngr_->getEntitiesByGroup(ecs::_grp_UI)) { //Renderizado de la interfaz
		if (e->isVisible()) {
			tex_ = mngr_->getComponent<Image>(e)->getTexture();
			auto tr_ = mngr_->getComponent<Transform>(e);
			SDL_Rect dest = build_sdlrect(tr_->getPos(), tr_->getW(), tr_->getH()); //Crea el rectángulo destino
			tex_->render(dest, tr_->getRotation()); //Renderiza la textura
			if (mngr_->hasComponent<TextBoxComponent>(e)) { //Si es un TextBox, le añade el texto además de la textura
				auto& sdl = *SDLUtils::instance();
				auto component = mngr_->getComponent<TextBoxComponent>(e);
				Vector2D pos_ = component->getPos();
				string text = component->getText();
				string display;
				if (text == "") display = component->getDisplay(); //En caso de no haber texto, escribe esto por defecto
				else display = text;
				Texture ipDirText(sdl.renderer(), display, sdl.fonts().at("ARIAL18"), build_sdlcolor(0x112233ff), build_sdlcolor(0xffffffff)); //Dibujado de textura
				ipDirText.render(pos_.getX(), pos_.getY());
			}
		}
	}
}

