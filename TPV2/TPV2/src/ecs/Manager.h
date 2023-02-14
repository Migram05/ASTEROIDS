#pragma once
#include <vector>
#include "../utils/Vector2D.h"
#include "../components/Transform.h"
#include "../components/Image.h"
#include "../components/FighterControl.h"
#include "../components/DeAcceleration.h"
#include "../components/ShowOpposite.h"
#include "../components/Health.h"
#include "../components/Gun.h"
#include "../components/DisableOnExit.h"
#include "../components/FramedImage.h"
#include "../components/Follow.h"
#include "../game/Game.h"
#include "../game/checkML.h"
using namespace std;
class Entity;
class Manager
{

public:
	Manager(Game* g);
	virtual ~Manager();
	Entity* addEntity();
	void refresh();
	void update();
	void render();
	void spawnShot(Vector2D pos, Vector2D dir, float rot);
	void createPlayer();
	inline Entity* getPlayer() { return player; }
	Texture* getTexture(int t);
	const int getWidth();
	const int getHeight();
	void exitGame();
private:
	Entity* player;
	vector<Entity*> ents_;
	Game* game;
	enum ValoresArray { GrayAsteroid, GoldAsteroid, Fighter1, Fighter2, Fire, Heart };
};

