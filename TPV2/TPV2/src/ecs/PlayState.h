#pragma once
#include "GameState.h"
#include "../utils/Vector2D.h"
#include "Manager.h"
#include "../components/Transform.h"
#include "../components/Image.h"
#include "Entity.h"
using namespace std;
class Game;
class PlayState : public GameState
{
public:
	PlayState(Game* g, double w, double h);
	~PlayState();
	virtual void update();
	virtual void render();
	virtual bool onEnter();
	virtual void refresh();

	virtual std::string getStateID() const { return s_playID; }
private:
	bool exit = false, gameOver = false, win = false, renderTime = false, startGame = false;
	static const string s_playID;
	int numLives = 0;
	list<GameObject*>::iterator ObjIt;
	Manager* manager_;
};

