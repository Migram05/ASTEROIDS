#pragma once
#include "GameState.h"
#include <SDL.h>
#include "../sdlutils/SDLUtils.h"
class Game;
class PauseState : public GameState
{
public:
	PauseState(Game* g, double w, double h);
	virtual ~PauseState();
	virtual void update();

	virtual bool onEnter();
	virtual std::string getStateID() const { return s_pauseID; }

private:
	Game* game;
	static const string s_pauseID;
};

