#pragma once
#include "../ecs/Entity.h"
using msgId_type = uint8_t;
enum msgId : msgId_type { //Identificadores de los mensajes
	_m_PAUSEGAME,
	_m_PLAYERLOST,
	_m_PLAYERWINS,
	_m_EXIT,
	_m_NEWGAME,
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

};

