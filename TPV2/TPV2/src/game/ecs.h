#pragma once
#include <cstdint>
class ecs
{
public:
	using cmpId_type = uint8_t;
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
};

