#pragma once
#include "Component.h"
#include "../ecs/Entity.h"
#include <SDL.h>
#include <iostream>
#include "../utils/Vector2D.h"
#include "../sdlutils/SDLUtils.h"
struct Gun : public Component
{
public:
	Gun();
	Gun(float s);
	Gun(float s, int shootR);
	virtual ~Gun();
	virtual void initComponent();
	inline float getSpeed() { return speed; }
	inline int getLastShotTime() { return lastShotTime; }
	inline int getShootRate() { return shootRate; }
	inline void setLastShoot(int lS) { lastShotTime = lS; }
	void shoot(Vector2D pos, Vector2D dir, float rot);
	constexpr static cmpId_type id = ecs::_GUN;
private:
	float speed;
	int lastShotTime, shootRate;
};


