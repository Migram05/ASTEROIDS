#pragma once
#include "GameState.h"
#include "../utils/Vector2D.h"
using namespace std;
class Game;
class PlayState : public GameState
{
public:
	PlayState(Game* g, double w, double h);
	~PlayState();
	virtual void update();
	virtual void render();
	virtual void handleEvents();
	virtual bool onEnter();

	virtual std::string getStateID() const { return s_playID; }
private:
	bool exit = false, gameOver = false, win = false, renderTime = false, startGame = false;
	static const string s_playID;
	int numLives = 0;
	list<GameObject*>::iterator ObjIt;
};

