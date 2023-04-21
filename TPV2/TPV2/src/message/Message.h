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
	_m_RESETPLAYERS,
	_m_SHOWALL,
	_m_SHIPSTATE,
};
struct Message { //Clase de los mensajes
	msgId_type id;
	struct {
		Entity* b_;
		Entity* a_;
	} bulletCollision_data;
	struct {
		string menuInfoData;
	} mainMenuInfo ;
	struct {
		Vector2D pos_;
		Vector2D dir_;
		int r_;
	}shot_data;
	struct {
		int idx;
		int pX;
		int pY;
		int R;
		bool S;
	} shipData;
	struct {
		bool resetLives = false;
	} resetShipsData;
};

