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
	void createAsteroids(int n);

private:
	void createNormalAsteroid();
	void createSeakingAsteroid();
	void createSmallerAsteroids(int n, int g, Entity* e);
	
	int numAsteroids, maxAsteroids = 11, maxDivision = 3;
	Manager* mngRef_;
	Entity* player_;
	enum ValoresArray { GrayAsteroid, GoldAsteroid, Fighter1, Fighter2, Fire, Heart };
};

