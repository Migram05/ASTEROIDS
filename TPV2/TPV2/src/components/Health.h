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
	inline int& getLives() { return numLives; }
	inline int getInitialLives() { return initialLives; }
	inline int getWidth() { return width_; }
	inline int getHeight() { return height_; }
	inline Texture* getTexture() { return tex_; }
	constexpr static cmpId_type id = ecs::_HEALTH;

#ifdef COMPS
	void render() override;
	void damage();
	void resetLives();
#endif // COMPS

private:
	Texture* tex_;
	int numLives, initialLives;
	int  Totwidth_, Totheight_, width_, height_, proportion = 40;
};

