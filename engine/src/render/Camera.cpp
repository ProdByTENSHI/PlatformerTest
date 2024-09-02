#include "render/Camera.h"

#include "core/Globals.h"

namespace tenshi
{
	Camera::Camera()
	{
		EventHandler<i32, i32> _onWindowResize([this](i32 width, i32 height)
			{
				CalculateOrtho(width, height);
			});
		g_OnWindowResize.Subscribe(_onWindowResize);
		CalculateOrtho(g_WindowWidth, g_WindowHeight);
	}

	void Camera::Update(Shader& shader)
	{
		if (glfwGetKey(g_Window, GLFW_KEY_W) == GLFW_PRESS)
			m_Position += glm::vec3(0.0f, m_Speed, 0.0f) * g_DeltaTime;
		if (glfwGetKey(g_Window, GLFW_KEY_S) == GLFW_PRESS)
			m_Position += glm::vec3(0.0f, -m_Speed, 0.0f) * g_DeltaTime;
		if (glfwGetKey(g_Window, GLFW_KEY_A) == GLFW_PRESS)
			m_Position += glm::vec3(-m_Speed, 0.0f, 0.0f) * g_DeltaTime;
		if (glfwGetKey(g_Window, GLFW_KEY_D) == GLFW_PRESS)
			m_Position += glm::vec3(m_Speed, 0.0f, 0.0f) * g_DeltaTime;

		if (glfwGetKey(g_Window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && m_Speed == INIT_SPEED)
			m_Speed *= 3.0f;
		else if (glfwGetKey(g_Window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
			m_Speed = INIT_SPEED;

		glm::mat4 _view = glm::lookAt(m_Position, m_Position + glm::vec3(0.0f, 0.0f, -1.0f), CAM_UP);

		shader.SetUniformMat4("u_ViewOrtho", m_Ortho + _view);
	}

	void Camera::CalculateOrtho(i32 width, i32 height)
	{
		m_Ortho = glm::ortho(0.0f, (f32)width, 0.0f, (f32)height, -1.0f, 1.0f);
	}
}