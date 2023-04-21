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
#include "../components/Button.h"
#include "../components/TextBoxComponent.h"
#include "../game/Game.h"
#include "../game/checkML.h"
#include "Entity.h"
#include "System.h"

using namespace std;
using grpId_type = uint8_t;
using sysId_type = uint8_t;
enum ValoresArray { GrayAsteroid, GoldAsteroid, Fighter1, Fire, Heart, OnePlayer, Multiplayer, Exit, Host, Join, TextBox };
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
		for (auto syst : sys_) delete syst;
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
					else { //SI está muerta, se borra la entidad
						delete e;
						return true;
					}
					}),
				grpEnts.end());
		}
	}


	void update() //Se actualizan todas las entidades
	{
#ifdef COMPS
		for (auto& ents : entsByGroup_) {
			auto n = ents.size();
			for (auto i = 0u; i < n; i++)
				ents[i]->update();
		}
#endif // COMPS
	}
	void render() //Se renderizan todas las entidades
	{
#ifdef COMPS
		for (auto& ents : entsByGroup_) {
			auto n = ents.size();
			for (auto i = 0u; i < n; i++)
				ents[i]->render();
		}
#endif // COMPS
	}
	void spawnShot(Vector2D pos, Vector2D dir, float rot) //Creación de un disparo
	{
		Entity* aux = addEntity(ecs::_grp_BULLETS); //Se añade al grupo de balas
		aux->setContext(this);
		addComponent<Transform>(aux, pos, 5, 30, dir, rot);
		addComponent<Image>(aux,game->getTexture(Fire));
		addComponent<DisableOnExit>(aux, game->WIN_WIDTH, game->WIN_HEIGHT);
	}
	void createPlayer(int n = 1) //Se crea al jugador
	{
		for (int x = 0; x < n; ++x) {
			//En este caso se guarda en una variable para poder referenciarla facilmente
			Entity* player = addEntity(ecs::_grp_PLAYER);
			//Escoge una posición según el número de jugadores
			if(n == 1) addComponent<Transform>(player, game->WIN_WIDTH / 2 - 15, game->WIN_HEIGHT /2 -15, 30, 30); //Se crea en el centro si hay 1 jugador
			else { //Si hay varios jugadores, se crean en distintas posiciones
				switch (x)
				{
				case 0: addComponent<Transform>(player, game->WIN_WIDTH / 2 - 15, game->WIN_HEIGHT * 0.9, 30, 30); break;
				case 1: addComponent<Transform>(player, game->WIN_WIDTH / 2 - 15, game->WIN_HEIGHT * 0.1, 30, 30); break;
				case 2: addComponent<Transform>(player, game->WIN_WIDTH * 0.1, game->WIN_HEIGHT / 2 - 15, 30, 30); break;
				case 3: addComponent<Transform>(player, game->WIN_WIDTH * 0.9, game->WIN_HEIGHT / 2 - 15, 30, 30); break;
				default: addComponent<Transform>(player, game->WIN_WIDTH + x, game->WIN_HEIGHT + x, 30, 30); break; //A partir de 4 jugadores las posiciones son según su índice
				}
			}
			addComponent<Image>(player, game->getTexture(Fighter1));
			addComponent<FighterControl>(player);
			addComponent<DeAcceleration>(player);
			addComponent<ShowOpposite>(player, game->WIN_WIDTH, game->WIN_HEIGHT);
			addComponent<Health>(player, game->getTexture(Heart), game->WIN_WIDTH, game->WIN_HEIGHT, 5);
			addComponent<Gun>(player, 10);
			players.push_back(player);
		}
	}
	inline Entity* getPlayer(int x = 0) { return players[x];}
	Texture* getTexture(int t) { return game->getTexture(t); }
	const int getWidth() { return game->WIN_WIDTH; }
	const int getHeight() { return game->WIN_HEIGHT; }

	//Devuelve el estado del jugador
	bool isPlayerAlive(int n = 0) { return getComponent<Health>(players[n])->getLives() > 0; }

	//Devuelve las entidades de un grupo
	const vector<Entity*>& getEntitiesByGroup(grpId_type gId = ecs::_grp_GENERAL) { return entsByGroup_[gId]; }
	const array<std::vector<Entity*>, ecs::maxGroupId>& getAllEntities() { return entsByGroup_; }
	//Añade entidad a un grupo
	void addToGroupList(grpId_type gId, Entity* e) { entsByGroup_[gId].push_back(e); }

	//Pausa el juego
	void pauseGame() { game->pauseGame(); }

	//Sale del juego
	void exitGame(string info) { game->exitToMenu(info); }

	//El jugador pierde
	void playerLost() { game->playerLoses(); }

	Entity* addEntity(grpId_type gId = ecs::_grp_GENERAL, bool vis = true)//Añade una entidad a su grupo correspondiente
	{
		Entity* e = new Entity(vis);
		e->alive_ = true;
		e->setContext(this);
		e->gId_ = gId;
		entsByGroup_[gId].push_back(e);
		return e;
	}
	template<typename T, typename ...Ts>
	inline T* addComponent(Entity* e, Ts&& ...args) { //Añade un componente a su grupo correspondiente
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
	inline T* addSystem(Ts &&... args) { //Añade un sistema
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

	inline void send(const Message& m, bool delay = false) { //Envío de mensajes
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
	inline void flushMessages() { //Envía todos los mensajes pendientes
		std::swap(msgs_, aux_msgs_); //Mueve los mensajes pendientes a una lista auxiliar
		for (auto& m : aux_msgs_) { 
			for (System* s : sys_) {
				if (s != nullptr) s->receive(m);
			}
		}
		aux_msgs_.clear(); //Se borra la lista auxiliar
	}
	inline Game* getGame() { return game; } //Referencia al game
	inline void setPlayerIndex(int i) { playerIndex = i; } //Ajusta el índice del jugador 
	inline int getPlayerIndex() { return playerIndex; } //Devuelve el índice
	inline void setPlayerName(string name) { playerName = name; } //Ajusta el nombre del jugador y del enemigo
	inline void setEnemyName(string name) { enemyName = name; }
	inline string getPlayerName() { return playerName; } //Devuelve los nombres
	inline string getEnemyName() { return enemyName; }
private:
	//Vector que almacena a los jugadores
	std::vector<Entity*> players ;
	int playerIndex = 0;
	string playerName, enemyName;
	Game* game;
	
	//Entidades
	std::array<std::vector<Entity*>, ecs::maxGroupId> entsByGroup_;
	//Sistemas
	std::array<System*, ecs::maxSystemId> sys_;
	//Mensajes
	std::vector<Message> msgs_;
	std::vector<Message> aux_msgs_;
};
