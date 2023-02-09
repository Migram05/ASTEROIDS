#pragma once
#include <list>
#include <string>
#include <iostream>
#include "GameObject.h"
#include "../utils/Vector2D.h"
using namespace std;
class Game;
class GameState
{
protected:
	list<GameObject*> objectsList;
	Game* game;
	enum ValoresArray { ball, blocks, nums, youLost, paddle, sidewall, topwall, youWin, reward, start, load, exitB, mainB, restart, resume, save};
	double WIN_WIDTH;
	double WIN_HEIGHT;
public:
	GameState(double w, double h);
	~GameState();
	virtual void update();
	void render();
	virtual void handleEvents() = 0;

	virtual bool onEnter() = 0;
	virtual string getStateID() const = 0;
};

