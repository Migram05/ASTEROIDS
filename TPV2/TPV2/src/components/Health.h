#pragma once
#include "Component.h"
#include "../sdlutils/Texture.h"
#include "Transform.h"
#include "../ecs/Entity.h"
struct Health : public Component
{
public:
	Health(Texture* text, int w, int h);
	Health(Texture* text, int w, int h, int n);
	virtual ~Health();
	void render() override;
	void damage();
	void resetLives();
	inline int& getLives() { return numLives; }
	constexpr static cmpId_type id = ecs::_HEALTH;
private:
	Texture* tex_;
	int numLives, initialLives;
	int  Totwidth_, Totheight_, width_, height_, proportion = 40;
};

