#pragma once
#include "GameState.h"
#include "../game/AsteroidsManager.h"
#include "../utils/Collisions.h"
class AsteroidsSystem;
class BulletsSystem;
class CollisionsSystem;
class FighterSystem;
class GameCtrlSystem;
class RenderSystem;
using namespace std;
class Manager;
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
	void checkCollisions();
	void resetGame();
	bool exit = false, gameOver = false, win = false, renderTime = false, startGame = false;
	static const string s_playID;
	Manager* manager_;
	AsteroidsManager* asteroidsManager_;

	AsteroidsSystem* asteroidSys_;
	BulletsSystem* bulletSys_;
	CollisionsSystem* collisionSys_;
	FighterSystem* fighterSys_;
	GameCtrlSystem* gameCtrlSys_;
	RenderSystem* renderSys_;
};

