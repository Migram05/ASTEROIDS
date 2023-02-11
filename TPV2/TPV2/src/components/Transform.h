#pragma once
#include "Component.h"
#include "../utils/Vector2D.h"
class Transform : public Component {
public:
	Transform();
	Transform(Vector2D pos, float w, float h);
	Transform(float x, float y, float w, float h);
	virtual ~Transform();
	inline Vector2D& getPos() { return position_; }
	inline Vector2D& getVel() { return velocity_; }
	inline float& getW() { return width_; }
	inline float& getH() { return height_; }
	inline float& getRotation() { return rotation_; }
	virtual void initComponent();
	virtual void update();
	virtual void render();
private:
	Vector2D position_;
	Vector2D velocity_;
	float width_;
	float height_;
	float rotation_;
};

