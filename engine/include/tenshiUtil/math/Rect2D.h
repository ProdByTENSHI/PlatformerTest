#pragma once

#include <glm/glm.hpp>

namespace tenshi
{
	struct Rect2D
	{
		glm::uvec2 m_Pos = glm::uvec2(0);
		glm::uvec2 m_Size = glm::uvec2(1);
	};
}