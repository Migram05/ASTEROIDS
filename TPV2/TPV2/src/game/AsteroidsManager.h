#pragma once
#include "../utils/Vector2D.h"
#include "../sdlutils/Texture.h"

class Entity;
class Manager;
class AsteroidsManager
{
public:
	AsteroidsManager(Manager* m, Entity* p);
	AsteroidsManager(Manager* m, Entity* p,  int n);

private:
	void createAsteroids(int n);
	void createNormalAsteroid();
	void createSeakingAsteroid();
	int numAsteroids;
	Manager* mngRef_;
	Entity* player_;
	enum ValoresArray { GrayAsteroid, GoldAsteroid, Fighter1, Fighter2, Fire, Heart };
};

