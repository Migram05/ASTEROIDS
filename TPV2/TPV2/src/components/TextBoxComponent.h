#pragma once
#include "Component.h"
#include "../sdlutils/Texture.h"
#include "../ecs/Entity.h"
class MainMenuState;
class TextBoxComponent : public Component
{
public:
	TextBoxComponent(MainMenuState* state, Vector2D position);
	virtual ~TextBoxComponent();
	void initComponent() override;
	inline Vector2D getPos() { return pos_; }
	inline string& getText() { return text; }
	constexpr static cmpId_type id = ecs::_TEXTBOX;
#ifdef COMPS
	void update() override;
	void render() override;
#endif // COMPS
private:
	Vector2D pos_;
	string text = "";
	MainMenuState* currentState;
};