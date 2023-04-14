#pragma once
#include "GameState.h"
using namespace std;
class Manager;
class RenderSystem;
class MenuControlSystem;
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
	void createButtons();
	static void startSingleplayer(Game* g);
	static void hostMultiplayer(Game* g);
	static void searchMultiplayer(Game* g);
	static void exitGame(Game* g);
	static void showButtonsClbck(Game* g);
	void showButtons();
	bool exit = false;
	static const string s_mainMID;
	float buttonW, buttonH;
	Manager* manager_;
	RenderSystem* renderSys_;
	MenuControlSystem* menuCtrlSys_;
};

