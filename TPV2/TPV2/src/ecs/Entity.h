#pragma once
#include "../game/ecs.h"
#include "Manager.h"/
#include "../components/Component.h"
#include <array>
#include <vector>
#include <bitset>

using namespace std;
using cmpId_type = uint8_t;
using grpId_type = uint8_t;
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
	inline void update() {
		auto n = currCmps_.size();
		for (auto i = 0u; i < n; i++) currCmps_[i]->update();
	}
	inline void render() {
		auto n = currCmps_.size();
		for (auto i = 0u; i < n; i++) currCmps_[i]->render();
	}
	template<typename T, typename ...Ts>
	inline T* addComponent(Ts&& ...args) {
		constexpr cmpId_type cId = T::id;
		T* c = new T(std::forward<Ts>(args)...);
		removeComponent<T>();
		currCmps_.push_back(c);
		cmps_[cId] = c;
		c->setContext(this, mngr_);
		c->initComponent();
		return c;
	}
	template<typename T>
	inline void removeComponent() {
		constexpr cmpId_type cId = T::id;
		if (cmps_[cId] != nullptr) {
			auto iter = find(currCmps_.begin(), currCmps_.end(), cmps_[cId]);
			currCmps_.erase(iter);
			delete cmps_[cId];
			cmps_[cId] = nullptr;
		}
	}
	template<typename T>
	inline T* getComponent() {
		constexpr cmpId_type cId = T::id;
		return static_cast<T*>(cmps_[cId]);
	}
	inline bool hasComponent(cmpId_type cId) {
		return cmps_[cId] != nullptr;
	}
	/*inline void addToGroup(grpId_type gId) {
		if (!groups_[gId]) {
			groups_[gId] = true;
			mngr_->addToGroupList(gId, this);
		}
	}
	inline void removeFromGroupList(grpId_type gId) {
		if (groups_[gId]) {
			groups_[gId] = false;
		}
	}
	inline bool hasGroup(grpId_type gId) {
		return groups_[gId];
	}*/
private:
	bool alive_;
	Manager* mngr_;
	vector<Component*> currCmps_;
	array<Component*, ecs::maxComponentId> cmps_;
	//std::bitset<ecs::maxGroupId> groups_;
};

