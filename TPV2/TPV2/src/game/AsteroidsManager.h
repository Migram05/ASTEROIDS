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
	void addAsteroidFrequency();
	inline void pauseTimer() { pause = true;  }

private:
	void createSmallerAsteroids(int n, int g, Entity* e);
	Vector2D borderSpawnLocation();
	int numAsteroids = 0, maxAsteroids = 30, maxDivision = 3, AsteroidTime = 5, timer_ = 0, defaultNum = 10;
	Manager* mngRef_;
	Entity* player_;
	enum ValoresArray { GrayAsteroid, GoldAsteroid, Fighter1, Fighter2, Fire, Heart };
	bool pause = false;
	float asteroidSpeed = 0.4;
};

