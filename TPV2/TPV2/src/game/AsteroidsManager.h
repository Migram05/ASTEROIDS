#pragma once
#include "../utils/Vector2D.h"
#include "../sdlutils/Texture.h"
#include "checkML.h"
class Entity;
class Manager;
class AsteroidsManager
{
public:
	AsteroidsManager(Manager* m, Entity* p);
	AsteroidsManager(Manager* m, Entity* p,  int n);
	void destroyAllAsteroids();
	void onCollision(Entity* e);

private:
	void createAsteroids(int n);
	void createNormalAsteroid();
	void createSeakingAsteroid();
	
	int numAsteroids, maxAsteroids = 30;
	Manager* mngRef_;
	Entity* player_;
	enum ValoresArray { GrayAsteroid, GoldAsteroid, Fighter1, Fighter2, Fire, Heart };
};

