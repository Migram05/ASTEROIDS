#pragma once
#include <SDL.h>
class GameObject
{

public:
	GameObject();
	virtual ~GameObject();
	virtual void render() = 0;
	virtual void update() = 0;
	virtual void handleEvents(SDL_Event event) = 0;
};

