#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace tenshi
{
	struct Transform2D
	{
		Transform2D()
		{
			Scale(1.0f);
		}

		Transform2D(const glm::vec2& position, const glm::vec2& scale)
		{
			Scale(scale);
			Translate(position);
		}

		void Translate(const glm::vec2& translation)
		{
			m_Position += translation;
			m_ModelMatrix = glm::translate(m_ModelMatrix, glm::vec3(translation.x, translation.y, 0.0f));
		}

		void Scale(const glm::vec2& scale)
		{
			m_Scale += scale;
			m_ModelMatrix = glm::translate(m_ModelMatrix, glm::vec3(scale.x, scale.y, 0.0f));
		}

		void Scale(f32 scalar)
		{
			Scale(glm::vec2(scalar, scalar));
		}

		glm::mat4 m_ModelMatrix = glm::mat4(1.0f);
		glm::vec2 m_Position = glm::vec2(0.0f);
		glm::vec2 m_Scale = glm::vec2(1.0f);
	};
}