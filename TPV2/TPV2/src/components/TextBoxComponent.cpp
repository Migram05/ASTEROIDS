#include "TextBoxComponent.h"
#include "../ecs/Manager.h"
#include "../states/MainMenuState.h"
TextBoxComponent::TextBoxComponent(MainMenuState* state, Vector2D position, string disp, bool readN) : currentState(state), pos_(position), display(disp), readNum(readN)
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
		if (event.type == SDL_KEYDOWN) { //Solo se buscan eventos de teclado
			char c = event.key.keysym.sym;
			if (event.key.keysym.sym-13 == 0) { //Cálculo para saber si es la tecla ENTER
				if(readNum) currentState->startMultiplayer(true, text); //Si es el textBox de IP, empieza el multijugador
				else {
					ent_->setAlive(false); //Sino, se destruye
					mngr_->setPlayerName(text); //Cambia el valor del nombre
					for (Entity* e : mngr_->getEntitiesByGroup(ecs::_grp_UI)) { //Muestra todas las entidades ocultas
						if (!e->isVisible()) e->setVisibility(true);
					}
				}
			}
			if (event.key.keysym.sym == SDLK_BACKSPACE && !text.empty()) { //Si se pulsa la barra de borrar, se elimina el último caracter
				auto it = text.end(); --it;
				text.erase(it);
			}
			else if (readNum) { //En caso de ser el textBox de IP, solo lee números
				if (isdigit(c) || c == '.') text += c; //Solo si es un número se añade
			}
			else if(text.size() <= 10) text += c; //Sino, lee cualquier caracter, hasta 10
		}
	}
}

void TextBoxComponent::render() //Renderizado del texto escrito
{
	auto& sdl = *SDLUtils::instance();
	string disp;
	if (text == "") disp = display; //En caso de no haber texto, escribe el display que le hayan pasado en la constructora
	else disp = text;
	Texture ipDirText(sdl.renderer(), disp, sdl.fonts().at("ARIAL18"), build_sdlcolor(0x112233ff), build_sdlcolor(0xffffffff)); //Dibujado de textura
	ipDirText.render(pos_.getX(), pos_.getY());
}
#endif // COMPS

TextBoxComponent::~TextBoxComponent()
{
}

