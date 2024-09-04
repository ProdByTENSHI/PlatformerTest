#include "input/InputManager.h"

#include <iostream>

#include "core/Globals.h"

namespace tenshi
{
	InputManager::InputManager()
		: m_CurrentlyPressedKeys(348, -1.0f)
	{
		m_CurrentlyPressedKeys.reserve(348);

		// Shitty Workaround but it works
		glfwSetKeyCallback(g_Window, [](GLFWwindow* window, i32 key, i32 scanCode, i32 action, i32 mods)
			{
				switch (action)
				{
				case GLFW_PRESS:
					g_InputManager->m_CurrentlyPressedKeys[key] = key;
					g_InputManager->m_KeyHoldData.insert(std::make_pair(key, 0.0f));
					g_InputManager->OnKeyDown.Dispatch(key);
					break;

				case GLFW_RELEASE:
					g_InputManager->m_CurrentlyPressedKeys.erase(g_InputManager->m_CurrentlyPressedKeys.begin() + key);
					g_InputManager->OnKeyUp.Dispatch(key);
					break;
				}
			});
	}

	void InputManager::Update()
	{
		for (i32 i = 0; i < m_CurrentlyPressedKeys.size(); i++)
		{
			if (m_CurrentlyPressedKeys[i] == -1)
				continue;

			m_KeyHoldData[i] += g_DeltaTime;
			if (m_KeyHoldData[i] < KEY_HOLD_DELAY)
				continue;

			OnKeyHold.Dispatch(m_CurrentlyPressedKeys[i]);
		}
	}
}