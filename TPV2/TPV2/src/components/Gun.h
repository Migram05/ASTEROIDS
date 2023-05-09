#pragma once
#include "../ecs/Component.h"
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
	constexpr static cmpId_type id = ecs::_GUN;
#ifdef COMPS
	void shoot(Vector2D pos, Vector2D dir, float rot);
#endif // COMPS

private:
	float speed;
	int lastShotTime, shootRate;
};


