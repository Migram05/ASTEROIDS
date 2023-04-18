#pragma once
#include "Component.h"
#include "../sdlutils/Texture.h"
#include "../ecs/Entity.h"
class MainMenuState;
class TextBoxComponent : public Component
{
public:
	TextBoxComponent(MainMenuState* state, Vector2D position, string display, bool readN);
	virtual ~TextBoxComponent();
	void initComponent() override;
	inline Vector2D getPos() { return pos_; }
	inline string& getText() { return text; }
	inline string getDisplay() { return display; }
	inline bool getReadNum() { return readNum; }
	constexpr static cmpId_type id = ecs::_TEXTBOX;
#ifdef COMPS
	void update() override;
	void render() override;
#endif // COMPS
private:
	Vector2D pos_;
	string text = "", display = "";
	MainMenuState* currentState;
	bool readNum;
};