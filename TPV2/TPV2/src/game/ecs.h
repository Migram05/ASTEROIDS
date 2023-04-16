#pragma once
#include <cstdint>
class ecs //Class ecs, controla los grupos, tipos de componentes y tipos de sistemas
{
public:
	using cmpId_type = uint8_t;
	using grpId_type = uint8_t;
	using sysId_type = uint8_t;
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
		_BUTTON,
		_TEXTBOX,
		// do not remove this
		_LAST_CMP_ID
	};
	static constexpr cmpId_type maxComponentId = _LAST_CMP_ID;
	
	enum grpId : grpId_type {
		_grp_GENERAL,
		_grp_ASTEROIDS,
		_grp_PLAYER,
		_grp_BULLETS,
		_grp_UI,
		//Do not remove this
		_LAST_GRP_ID
	};
	static constexpr grpId_type maxGroupId = _LAST_GRP_ID;

	enum sysId : sysId_type {
		_sys_ASTEROIDS,
		_sys_BULLET,
		_sys_COLLISION,
		_sys_FIGHTER,
		_sys_GAMECTRL,
		_sys_RENDER,
		_sys_MENUCONTROL,
		//Do not remove this
		_LAST_SYS_ID
	};
	static constexpr sysId_type maxSystemId = _LAST_SYS_ID;
};

