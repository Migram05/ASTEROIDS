#pragma once
#include "../utils/Vector2D.h"
#include "../sdlutils/Texture.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
class AsteroidsManager
{
public:
	AsteroidsManager(Manager* m);
	AsteroidsManager(Manager* m, int n);

private:
	void createAsteroids(int n);
	int numAsteroids;
	Manager* mngRef_;
	Entity* player_;
	enum ValoresArray { GrayAsteroid, GoldAsteroid, Fighter1, Fighter2, Fire, Heart };
};

