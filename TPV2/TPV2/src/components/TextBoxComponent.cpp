#include "TextBoxComponent.h"
#include "../ecs/Manager.h"
#include "../ecs/MainMenuState.h"
TextBoxComponent::TextBoxComponent(MainMenuState* state, Vector2D position) : currentState(state), pos_(position)
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
				currentState->startMultiplayer(text);
			}
			if (event.key.keysym.sym == SDLK_BACKSPACE && !text.empty()) {
				auto it = text.end(); --it;
				text.erase(it);
			}
			if(isdigit(c) || c=='.') text += c; //Solo si es un número se añade
			
			cout << text << endl;
		}
	}
}

void TextBoxComponent::render() //Renderizado de la ip escrita
{
	auto& sdl = *SDLUtils::instance();
	string display;
	if (text == "") display = "ESCRIBA LA DIRECCION IP"; //En caso de no haber texto, escribe esto por defecto
	else display = text;
	Texture ipDirText(sdl.renderer(), display, sdl.fonts().at("ARIAL18"), build_sdlcolor(0x112233ff), build_sdlcolor(0xffffffff)); //Dibujado de textura
	ipDirText.render(pos_.getX(), pos_.getY());
}
#endif // COMPS

