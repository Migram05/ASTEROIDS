#pragma once
#include "GameState.h"
using namespace std;
class Manager;
class Entity;
class RenderSystem;
class MenuControlSystem;
class MainMenuState : public GameState
{
public:
	MainMenuState(Game* g, double w, double h, string infoMsg = "");
	~MainMenuState();
	virtual void update();
	virtual void render();
	virtual bool onEnter();
	virtual void refresh();
	void startMultiplayer(bool c, string dir);
	inline void deactivateTextBox() { textBoxEnt = nullptr; }
	inline Entity* getTextBox() { return textBoxEnt; }
	inline string getInfo() { return infoText; }
	virtual std::string getStateID() const { return s_mainMID; }
private:
	void createButtons();
	static void startSingleplayer(Game* g);
	static void hostMultiplayer(Game* g);
	static void searchMultiplayer(Game* g);
	
	static void exitGame(Game* g);
	static void enterName(Game* g);
	void showButtons();
	void startRead(string dTxt, bool readN = true);
	bool exit = false;
	static const string s_mainMID;
	float buttonW, buttonH, txtBoxW, txtBoxH;
	Entity* textBoxEnt = nullptr;
	Manager* manager_;
	RenderSystem* renderSys_;
	MenuControlSystem* menuCtrlSys_;
	string infoText;
};

