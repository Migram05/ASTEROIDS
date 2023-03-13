#pragma once
//#include "../game/ecs.h"
//#include "Manager.h"/
#include "../components/Component.h"
#include <array>
#include <vector>
#include <bitset>

using namespace std;
using cmpId_type = uint8_t;
using grpId_type = uint8_t;
class Manager;
class Entity
{
public:
	Entity();
	Entity(const Entity&) = delete;
	Entity& operator=(const Entity&) = delete;
	void setContext(Manager* mngr);
	virtual ~Entity() { //Destructora, borra todos los componentes
		for (auto c : currCmps_) {
			delete c;
		}
	}
#ifdef COMPS
	
	inline bool isAlive() { return alive_; } //Devuelve el estado de la entidad
	inline void setAlive(bool alive) { alive_ = alive; } //Ajusta su valor
	inline void update() { //Update de todos los componentes
		auto n = currCmps_.size();
		for (auto i = 0u; i < n; i++) currCmps_[i]->update();
	}
	inline void render() { //Render de todos los componentes
		auto n = currCmps_.size();
		for (auto i = 0u; i < n; i++) currCmps_[i]->render();
	}
	template<typename T, typename ...Ts>
	inline T* addComponent(Ts&& ...args) { //Añade un componente a su grupo correspondiente
		constexpr cmpId_type cId = T::id; //Obtiene el grupo a partir del tipo
		T* c = new T(std::forward<Ts>(args)...);
		removeComponent<T>(); //Quita los componentes del mismo tipo que haya
		currCmps_.push_back(c);
		cmps_[cId] = c;
		c->setContext(this, mngr_); //Inicia el componente
		c->initComponent();
		return c;
	}
	template<typename T>
	inline void removeComponent() { //Borra un componente de su grupo correspondiente
		constexpr cmpId_type cId = T::id; //Obtiene el grupo
		if (cmps_[cId] != nullptr) { //En caso de haber componentes en el grupo, lo borra
			auto iter = find(currCmps_.begin(), currCmps_.end(), cmps_[cId]);
			currCmps_.erase(iter);
			delete cmps_[cId];
			cmps_[cId] = nullptr;
		}
	}
	template<typename T>
	inline T* getComponent() { //Devuelve el componente del tipo
		constexpr cmpId_type cId = T::id;
		return static_cast<T*>(cmps_[cId]);
	}
	inline bool hasComponent(cmpId_type cId) { //Devuekve si hay un componente X
		return cmps_[cId] != nullptr;
	}
	
#endif // COMPS
	
private:
	friend Manager;
	bool alive_; //Variable que marca a la entidad como pendiente para borrar
	Manager* mngr_;
	vector<Component*> currCmps_;
	array<Component*, ecs::maxComponentId> cmps_;
	grpId_type gId_;
};

