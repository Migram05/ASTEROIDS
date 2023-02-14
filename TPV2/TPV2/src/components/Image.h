#pragma once
#include "Component.h"
#include "../sdlutils/Texture.h"
#include "Transform.h"
#include "../ecs/Entity.h"
class Image : public Component {
public:
	Image(Texture* text);
	virtual ~Image();
	void initComponent() override;
	void render() override;
private:
	Transform* tr_;
	Texture* tex_;
};
