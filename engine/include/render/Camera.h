#pragma once

#include <glm/glm.hpp>

#include "resources/Shader.h"
#include "tenshiUtil/Types.h"

namespace tenshi
{
	class Camera
	{
	public:
		Camera();

		void Update(Shader& shader);

	private:
		const f32 INIT_SPEED = 1.0f;
		f32 m_Speed = INIT_SPEED;

		glm::vec3 m_Position = glm::vec3(0.0f);
		glm::mat4 m_Ortho = glm::mat4(1.0f);

		const glm::vec3 CAM_UP = glm::vec3(0.0f, 1.0f, 0.0f);

	private:
		void CalculateOrtho(i32 width, i32 height);
	};
}