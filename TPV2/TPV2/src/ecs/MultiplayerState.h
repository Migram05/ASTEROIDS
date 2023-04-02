#pragma once
#include "GameState.h"
#include "../utils/Collisions.h"
#include <SDL_net.h>

class BulletsSystem;
class CollisionsSystem;
class FighterSystem;
class GameCtrlSystem;
class RenderSystem;
class Manager;
using namespace std;
class MultiplayerState : public GameState
{
public:
	MultiplayerState(Game* g, double w, double h);
	~MultiplayerState();
	virtual void update();
	virtual void render();
	virtual bool onEnter();
	virtual void refresh();
	void resetGame();
	virtual std::string getStateID() const { return s_playID; }
private:
	void checkCollisions();

	bool exit = false, gameOver = false, win = false, renderTime = false, startGame = false;
	static const string s_playID;
	Manager* manager_;

	BulletsSystem* bulletSys_;
	CollisionsSystem* collisionSys_;
	FighterSystem* fighterSys_;
	GameCtrlSystem* gameCtrlSys_;
	RenderSystem* renderSys_;
};
