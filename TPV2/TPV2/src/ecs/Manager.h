#pragma once
#include <vector>
#include <array>
#include "../utils/Vector2D.h"
#include "../components/Transform.h"
#include "../components/Image.h"
#include "../components/FighterControl.h"
#include "../components/DeAcceleration.h"
#include "../components/ShowOpposite.h"
#include "../components/Health.h"
#include "../components/Gun.h"
#include "../components/DisableOnExit.h"
#include "../components/FramedImage.h"
#include "../components/Follow.h"
#include "../components/Generations.h"
#include "../game/Game.h"
#include "../game/checkML.h"
#include "Entity.h"
#include "System.h"

using namespace std;
using grpId_type = uint8_t;
using sysId_type = uint8_t;
//class Entity;
class Manager
{
public:
	Manager(Game* g) : entsByGroup_(),sys_(), game(g) //Constructora
	{
		for (auto& groupEntities : entsByGroup_) { //Se reserva memoria
			groupEntities.reserve(100);
		}
	}
	virtual ~Manager() { //Destructora
		for (auto& ents : entsByGroup_) { //Recorre todos los grupos
			for (auto e : ents)
				delete e;
		}
	}
	void refresh() {
		//Borra todas las entidades muertas
		for (ecs::grpId_type gId = 0; gId < ecs::maxGroupId; gId++) { //Pasa por los distintos grupos
			auto& grpEnts = entsByGroup_[gId];
			grpEnts.erase(
				std::remove_if(grpEnts.begin(), grpEnts.end(), [](Entity* e) {
					if (e->alive_) {
						return false;
					}
					else { //SI est� muerta, se borra la entidad
						delete e;
						return true;
					}
					}),
				grpEnts.end());
		}
	}
	void update() //Se actualizan todas las entidades
	{
		for (auto& ents : entsByGroup_) {
			auto n = ents.size();
			for (auto i = 0u; i < n; i++);
				//ents[i]->update();
		}
	}
	void render() //Se renderizan todas las entidades
	{
		for (auto& ents : entsByGroup_) {
			auto n = ents.size();
			for (auto i = 0u; i < n; i++);
				//ents[i]->render();
		}
	}
	void spawnShot(Vector2D pos, Vector2D dir, float rot) //Creaci�n de un disparo
	{
		Entity* aux = addEntity(ecs::_grp_BULLETS); //Se a�ade al grupo de balas
		aux->setContext(this);
		addComponent<Transform>(aux, pos, 5, 30, dir, rot);
		addComponent<Image>(aux,game->getTexture(Fire));
		addComponent<DisableOnExit>(aux, game->WIN_WIDTH, game->WIN_HEIGHT);
	}
	void createPlayer() //Se crea al jugador
	{
		//EN este caso se guarda en una variable para poder referenciarla facilmente
		player = addEntity(ecs::_grp_PLAYER);
		addComponent<Transform>(player, game->WIN_WIDTH / 2 - 15, game->WIN_HEIGHT / 2 - 15, 30, 30);
		addComponent<Image>(player, game->getTexture(Fighter1));
		addComponent<FighterControl>(player);
		addComponent<DeAcceleration>(player);
		addComponent<ShowOpposite>(player, game->WIN_WIDTH, game->WIN_HEIGHT);
		addComponent<Health>(player, game->getTexture(Heart), game->WIN_WIDTH, game->WIN_HEIGHT, 5);
		addComponent<Gun>(player, 10);
	}
	inline Entity* getPlayer() { return player; }
	Texture* getTexture(int t) { return game->getTexture(t); }
	const int getWidth() { return game->WIN_WIDTH; }
	const int getHeight() { return game->WIN_HEIGHT; }

	//Devuelve el estado del jugador
	bool isPlayerAlive() { return getComponent<Health>(player)->getLives() > 0; }

	//Devuelve las entidades de un grupo
	const vector<Entity*>& getEntitiesByGroup(grpId_type gId = ecs::_grp_GENERAL) { return entsByGroup_[gId]; }
	const array<std::vector<Entity*>, ecs::maxGroupId>& getAllEntities() { return entsByGroup_; }
	//A�ade entidad a un grupo
	void addToGroupList(grpId_type gId, Entity* e) { entsByGroup_[gId].push_back(e); }

	//Pausa el juego
	void pauseGame() { game->pauseGame(); }

	//Sale del juego
	void exitGame() { game->exitGame(); }

	//El jugador pierde
	void playerLost() { game->playerLoses(); }

	Entity* addEntity(grpId_type gId = ecs::_grp_GENERAL)//A�ade una entidad a su grupo correspondiente
	{
		Entity* e = new Entity();
		e->alive_ = true;
		e->setContext(this);
		e->gId_ = gId;
		entsByGroup_[gId].push_back(e);
		return e;
	}
	template<typename T, typename ...Ts>
	inline T* addComponent(Entity* e, Ts&& ...args) { //A�ade un componente a su grupo correspondiente
		T* c = new T(std::forward<Ts>(args)...);
		constexpr cmpId_type cId = T::id;
		removeComponent<T>(e);
		e->currCmps_.push_back(c);
		e->cmps_[cId] = c;
		c->setContext(e, this);
		c->initComponent();
		return c;
	}

	template<typename T>
	inline void removeComponent(Entity* e) { //Borra el componente de una entidad
		constexpr cmpId_type cId = T::id;
		if (e->cmps_[cId] != nullptr) {
			auto iter = std::find(e->currCmps_.begin(),
				e->currCmps_.end(),
				e->cmps_[cId]);
			e->currCmps_.erase(iter);
			delete e->cmps_[cId];
			e->cmps_[cId] = nullptr;
		}
	}
	template<typename T>
	inline bool hasComponent(Entity* e) { //Comprueba si una entidad tiene un componente
		constexpr cmpId_type cId = T::id;
		return e->cmps_[cId] != nullptr;
	}
	template<typename T>
	inline T* getComponent(Entity* e) { //Obtiene  el componente de un cierto tipo de una entidad
		constexpr cmpId_type cId = T::id;
		return static_cast<T*>(e->cmps_[cId]);
	}
	inline void setAlive(Entity* e, bool alive) { //Ajusta el estado de una entidad
		e->alive_ = alive;
	}
	inline bool isAlive(Entity* e) { //Obtiene el estado de la entidad
		return e->alive_;
	}
	inline grpId_type groupId(Entity* e) { //Obtiene el grupo al que pertenece una entidad
		return e->gId_;
	}
	template<typename T, typename ...Ts>
	inline T* addSystem(Ts &&... args) { //A�ade un sistema
		System* s = new T(std::forward<Ts>(args)...);
		constexpr sysId_type sId = T::id;
		removeSystem<T>();
		s->setContext(this);
		s->initSystem();
		sys_[sId] = s;
		return static_cast<T*>(s);
	}
	template<typename T>
	inline T* getSystem() { //Devuelve un sistema concreto
		constexpr sysId_type sId = T::id;
		return static_cast<T*>(sys_[sId]);
	}
	template<typename T>
	inline void removeSystem() { //Borra sistema 
		constexpr sysId_type sId = T::id;
		if (sys_[sId] != nullptr) {
			delete sys_[sId];
			sys_[sId] = nullptr;
		}
	}

	inline void send(const Message& m, bool delay = false) { //Env�o de mensajes
		if (!delay) {
			for (System* s : sys_) {
				if (s != nullptr)
					s->receive(m);
			}
		}
		else { //En caso de enviar con delay se mueven a la lista de mensajes pendientes
			msgs_.emplace_back(m);
		}
	}
	inline void flushMessages() { //Env�a todos los mensajes pendientes
		std::swap(msgs_, aux_msgs_); //Mueve los mensajes pendientes a una lista auxiliar
		for (auto& m : aux_msgs_) { 
			for (System* s : sys_) {
				if (s != nullptr) s->receive(m);
			}
		}
		aux_msgs_.clear(); //Se borra la lista auxiliar
	}
private:
	Entity* player;
	Game* game;
	enum ValoresArray { GrayAsteroid, GoldAsteroid, Fighter1, Fire, Heart };
	//Entidades
	std::array<std::vector<Entity*>, ecs::maxGroupId> entsByGroup_;
	//Sistemas
	std::array<System*, ecs::maxSystemId> sys_;
	//Mensajes
	std::vector<Message> msgs_;
	std::vector<Message> aux_msgs_;
};
