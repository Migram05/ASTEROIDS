#pragma once
#include <cstdint>
class ecs
{
public:
	using cmpId_type = uint8_t;
	enum cmpId : cmpId_type {
		_TRANSFORM = 0,
		_IMAGE,
		_CTRL,
		// do not remove this
		_LAST_CMP_ID
	};
	static constexpr cmpId_type maxComponentId = _LAST_CMP_ID;
};

