#pragma once
#include "Component.h"
#include "../sdlutils/Texture.h"
#include "Transform.h"
#include "../ecs/Entity.h"
struct Image : public Component {
public:
	Image(Texture* text);
	virtual ~Image();
	void initComponent() override;
	inline Texture* getTexture() { return tex_; }
	constexpr static cmpId_type id = ecs::_IMAGE;

#ifdef COMPS
	void render() override;
#endif // COMPS

private:
	Transform* tr_;
	Texture* tex_;
};

