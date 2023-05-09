#pragma once
#include "GameState.h"
#include <SDL.h>
#include <string>
#include "../sdlutils/SDLUtils.h"
class Game;
class PauseState : public GameState
{
public:
	PauseState(Game* g, double w, double h, bool l);
	virtual ~PauseState();
	virtual void update();
	virtual void render();
	virtual bool onEnter();
	virtual std::string getStateID() const { return s_pauseID; }

private:
	Game* game;
	static const string s_pauseID;
	bool lost; //Usada para saber si hay que renderizar la pantalla de derrota
};

