#pragma once

#include <glm/glm.hpp>

#include "tenshiUtil/Types.h"
#include "tenshiUtil/eventsystem/EventSystem.h"

namespace tenshi
{
	// Holds all relevant Input Data for the Player
	struct InputData
	{
		glm::vec2 m_MovementDirection;

		bool m_IsCrouching;
		bool m_CanJump = false;
	};
}