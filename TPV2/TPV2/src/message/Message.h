#pragma once
#include "../ecs/Entity.h"
#include "../utils/Vector2D.h"
using msgId_type = uint8_t;
enum msgId : msgId_type { //Identificadores de los mensajes
	_m_PAUSEGAME,
	_m_PLAYERLOST,
	_m_PLAYERWINS,
	_m_EXIT,
	_m_NEWGAME,
	_m_SHOOT,
	_m_BULLETCOLLIDES,
	_m_RESUME,
	_m_CHANGEINDEX,
	_m_MOVESHIP,
	_m_ROTATESHIP,
	_m_SHIPSHOOT,
};
struct Message { //Clase de los mensajes
	msgId_type id;
	// _m_STAR_EATEN
	struct {
		Entity* e;
	} star_eaten_data;
	// _m_ADD_STARS
	struct {
		unsigned int n;
	} add_stars_data;
	struct {
		Vector2D pos_;
		Vector2D dir_;
		float r_;
	} shot_data;
	struct {
		Entity* b_;
		Entity* a_;
	} bulletCollision_data;
	struct {
		int indx;
	} moveShip_data;
	struct {
		int indx;
		int proportion;
	} rotateShip_data;
	struct {
		int indx;
	} shipShoot_data;
};

