#pragma once
#include "Component.h"
#include "../ecs/Entity.h"
#include <SDL.h>
#include <iostream>
#include "../utils/Vector2D.h"
#include "../sdlutils/SDLUtils.h"
class Gun : public Component
{
public:
	Gun();
	Gun(float s);
	Gun(float s, int shootR);
	virtual ~Gun();
	virtual void initComponent();
	void shoot(Vector2D pos, Vector2D dir);
private:
	float speed;
	int lastShotTime, shootRate;
};

