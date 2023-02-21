#pragma once
#include <cstdint>
class ecs //Class ecs, controla los grupos y tipos de componentes
{
public:
	using cmpId_type = uint8_t;
	using grpId_type = uint8_t;
	enum cmpId : cmpId_type {
		_TRANSFORM,
		_DEACCELERATION,
		_IMAGE,
		_HEALTH,
		_CTRL,
		_GUN,
		_SHOWOPOSITE,
		_FRAMEDIMAGE,
		_GENERATIONS,
		_FOLLOW,
		_DISABLEONEXIT,
		// do not remove this
		_LAST_CMP_ID
	};
	static constexpr cmpId_type maxComponentId = _LAST_CMP_ID;
	
	enum grpId : grpId_type {
		_grp_GENERAL,
		_grp_ASTEROIDS,
		_grp_PLAYER,
		_grp_BULLETS,
		//Do not remove this
		_LAST_GRP_ID
	};
	static constexpr grpId_type maxGroupId = _LAST_GRP_ID;
};

