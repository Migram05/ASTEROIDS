#pragma once
#include "GameState.h"
using namespace std;
class Manager;
class RenderSystem;
class MainMenuState : public GameState
{
public:
	MainMenuState(Game* g, double w, double h);
	~MainMenuState();
	virtual void update();
	virtual void render();
	virtual bool onEnter();
	virtual void refresh();

	virtual std::string getStateID() const { return s_mainMID; }
private:
	bool exit = false;
	static const string s_mainMID;
	Manager* manager_;
	RenderSystem* renderSys_;
};

