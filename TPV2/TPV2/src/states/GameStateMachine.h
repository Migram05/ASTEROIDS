#pragma once
#include <stack>
#include "GameState.h"
using namespace std;
class GameStateMachine
{
private:
	stack<GameState*> stateStack;
	list<GameState*> toDeleteList;
	GameState* secondary = nullptr;
public:
	GameStateMachine();
	~GameStateMachine();
	void update();
	void render();
	GameState* currentState();
	void pushState(GameState* pState);
	void changeState(GameState* pState);
	void popState();
	void clearList();
};

