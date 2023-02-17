#pragma once
#include "Component.h"
#include "../utils/Vector2D.h"
#include <iostream>
class Transform : public Component {
public:
	Transform();
	Transform(Vector2D pos, float w, float h);
	Transform(float x, float y, float w, float h);
	Transform(Vector2D pos, float w, float h, Vector2D vel, float rot = 0);
	Transform(float x, float y, float w, float h, Vector2D vel, float rot = 0);
	virtual ~Transform();
	inline Vector2D& getPos() { return position_; }
	inline Vector2D& getVel() { return velocity_; }
	inline Vector2D& getForward() { return forward_; }
	inline Vector2D& getLastForward() { return lastForward_; }
	inline float& getW() { return width_; }
	inline float& getH() { return height_; }
	inline float& getRotation() { return rotation_; }
	virtual void initComponent();
	virtual void update();
	virtual void render(); 
	constexpr static cmpId_type id = ecs::_TRANSFORM;
private:
	Vector2D position_;
	Vector2D velocity_;
	Vector2D forward_ = { 0, -1 };
	Vector2D lastForward_ = { 0, -1 };
	float width_;
	float height_;
	float rotation_;
};

