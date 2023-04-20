#include "TextBoxComponent.h"
#include "../ecs/Manager.h"
#include "../ecs/MainMenuState.h"
TextBoxComponent::TextBoxComponent(MainMenuState* state, Vector2D position, string disp, bool readN) : currentState(state), pos_(position), display(disp), readNum(readN)
{

}

TextBoxComponent::~TextBoxComponent()
{
}

void TextBoxComponent::initComponent()
{
}


#ifdef COMPS

void TextBoxComponent::update()
{
	SDL_Event event;
	auto& sdl = *SDLUtils::instance();
	while (SDL_PollEvent(&event)) { //Controla la entrada
		if (event.type == SDL_KEYDOWN) {
			char c = event.key.keysym.sym;
			if (event.key.keysym.sym-13 == 0) { //Cálculo para saber si es la tecla ENTER
				if(readNum) currentState->startMultiplayer(true, text);
				else {
					ent_->setAlive(false);
					mngr_->setPlayerName(text);
					for (Entity* e : mngr_->getEntitiesByGroup(ecs::_grp_UI)) {
						if (!e->isVisible()) e->setVisibility(true);
					}
				}
			}
			if (event.key.keysym.sym == SDLK_BACKSPACE && !text.empty()) {
				auto it = text.end(); --it;
				text.erase(it);
			}
			else if (readNum) {
				if (isdigit(c) || c == '.') text += c; //Solo si es un número se añade
			}
			else if(text.size() <= 10) text += c;
		}
	}
}

void TextBoxComponent::render() //Renderizado de la ip escrita
{
	auto& sdl = *SDLUtils::instance();
	string disp;
	if (text == "") disp = display; //En caso de no haber texto, escribe esto por defecto
	else disp = text;
	Texture ipDirText(sdl.renderer(), disp, sdl.fonts().at("ARIAL18"), build_sdlcolor(0x112233ff), build_sdlcolor(0xffffffff)); //Dibujado de textura
	ipDirText.render(pos_.getX(), pos_.getY());
}
#endif // COMPS

