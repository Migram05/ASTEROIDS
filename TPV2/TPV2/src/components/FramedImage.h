#pragma once
#include "Component.h"
#include "../sdlutils/Texture.h"
#include "Transform.h"
#include "../ecs/Entity.h"
#include <SDL.h>
struct FramedImage : public Component
{
public:
	FramedImage(Texture* text, int r, int c);
	FramedImage(Texture* text, int r, int c, int f);
	virtual ~FramedImage();
	void initComponent() override;
	void render() override;
	inline int getNRows() { return numRows_; }
	inline int getNCols() { return numCols_; }
	constexpr static cmpId_type id = ecs::_FRAMEDIMAGE;
private:
	Transform* tr_;
	Texture* tex_;
	int numRows_, numCols_;
	int row = 0, col = 0;
	int tPerFrame;
};

