#pragma once
#include "GameState.h"
#include "../utils/Collisions.h"
#include <SDL_net.h>

class BulletsSystem;
class CollisionsSystem;
class FighterSystemOnline;
class GameCtrlSystem;
class RenderSystem;
class Manager;
using namespace std;
class MultiplayerState : public GameState
{
public:
	MultiplayerState(Game* g, double w, double h, bool c,  string name,  string ipDir = "localhost");
	~MultiplayerState();
	virtual void update();
	virtual void render();
	virtual bool onEnter();
	virtual void refresh();
	void sendMessage(string m);
	virtual std::string getStateID() const { return s_playID; }
private:
	void onRecieveMessage(char* m);
	void checkExit();
	bool exit = false, gameOver = false, win = false, renderTime = false, startGame = false, isClient;
	string ipDir;
	static const string s_playID;
	Manager* manager_ = nullptr;
	IPaddress ip;
	int port = 5555;
	int playerIndex = 0;
	TCPsocket master_socket;
	TCPsocket client = NULL;
	SDLNet_SocketSet socketSet = SDLNet_AllocSocketSet(2);

	BulletsSystem* bulletSys_;
	CollisionsSystem* collisionSys_;
	FighterSystemOnline* fighterSys_;
	GameCtrlSystem* gameCtrlSys_;
	RenderSystem* renderSys_;
};
