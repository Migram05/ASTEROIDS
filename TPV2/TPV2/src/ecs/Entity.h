#pragma once
#include "../game/ecs.h"
#include "Manager.h"
#include "../components/Component.h"
#include <array>
#include <vector>

using namespace std;
using cmpId_type = uint8_t;

class Entity
{
public:
	Entity() : mngr_(nullptr), cmps_(), currCmps_(), alive_()
	{
		currCmps_.reserve(ecs::maxComponentId);
	}
	virtual ~Entity() {
		for (auto c : currCmps_) {
			delete c;
		}
	}
	inline void setContext(Manager* mngr) {
		mngr_ = mngr;
	}
	inline bool isAlive() { return alive_; }
	inline void setAlive(bool alive) { alive_ = alive; }
	inline void update(){
		auto n = currCmps_.size();
		for (auto i = 0u; i < n; i++) currCmps_[i]->update();
	}
	inline void render(){
		auto n = currCmps_.size();
		for (auto i = 0u; i < n; i++) currCmps_[i]->render();
	}
	template<typename T, typename ...Ts>
	inline T* addComponent(cmpId_type cId, Ts&& ...args) {
		T* c = new T(std::forward<Ts>(args)...);
		removeComponent(cId);
		currCmps_.push_back(c);
		cmps_[cId] = c;
		c->setContext(this, mngr_);
		c->initComponent();
		return c;
	}
	inline void removeComponent(cmpId_type cId) {
		if (cmps_[cId] != nullptr) {
			auto iter = find(currCmps_.begin(), currCmps_.end(), cmps_[cId]);
			currCmps_.erase(iter);
			delete cmps_[cId];
			cmps_[cId] = nullptr;
		}
	}
	template<typename T>
	inline T* getComponent(cmpId_type cId) {
		return static_cast<T*>(cmps_[cId]);
	}
	inline bool hasComponent(cmpId_type cId) {
		return cmps_[cId] != nullptr;
	}
private:
	bool alive_;
	Manager* mngr_;
	vector<Component*> currCmps_;
	array<Component*, ecs::maxComponentId> cmps_;
};

