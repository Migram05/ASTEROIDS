#pragma once
#include <list>
#include <string>
#include "../utils/Vector2D.h"
using namespace std;
class Game;
class GameState
{
protected:
	Game* game;
	double WIN_WIDTH;
	double WIN_HEIGHT;
public:
	GameState(double w, double h);
	virtual ~GameState();
	virtual void update();
	virtual void render();
	virtual void refresh();

	virtual bool onEnter() = 0;
	virtual string getStateID() const = 0;
};

