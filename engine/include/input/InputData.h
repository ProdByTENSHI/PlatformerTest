#pragma once

#include <glm/glm.hpp>

#include "tenshiUtil/Types.h"
#include "tenshiUtil/eventsystem/EventSystem.h"

namespace tenshi
{
	// Base Class for all Input Data
	struct InputData
	{
		glm::vec2 m_MovementDirection;
		f32 m_MovementSpeed = 0.0f;

		bool m_CanJump = false;
	};
}